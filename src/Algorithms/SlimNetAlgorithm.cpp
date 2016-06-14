/******************************************************************************

    source file SlimNetAlgorithm.cpp for class: SlimNetAlgorithm

    See header file for details

    Author : Suraj Ketan Samal
    Version: 1
    Date   : June 7, 2016 

******************************************************************************/


// Include(s)
#include "Algorithms/Algorithm.h"
#include "Algorithms/SlimNetAlgorithm.h"
#include "Network/Flow.h"
#include "Network/LinkList.h"
#include "Network/Path.h"
#include "Network/Topology.h"
#include "Utils/TraceManager.h"
#include <cfloat>
#include <cmath>
#include <vector>
using std::vector;

// Constanst(s)

// Variable(s)

// Function(s) definitions

//------------------------------------------------------------------------------
//  constructor SlimNetAlgorithm::SlimNetAlgorithm()
//------------------------------------------------------------------------------
SlimNetAlgorithm::SlimNetAlgorithm(const TString &args) :
                   Algorithm(args)
{
    TRACE("SlimNetAlgorithm::SlimNetAlgorithm -->");
    rng = RandomNumberGenerator::getRandomNumberGenerator();
    TRACE("SlimNetAlgorithm::SlimNetAlgorithm <--");
}


//------------------------------------------------------------------------------
//  destructor SlimNetAlgorithm::~SlimNetAlgorithm()
//------------------------------------------------------------------------------
SlimNetAlgorithm::~SlimNetAlgorithm()
{
    TRACE("SlimNetAlgorithm::~SlimNetAlgorithm -->");
    // Empty
    TRACE("SlimNetAlgorithm::~SlimNetAlgorithm <--");
}

//------------------------------------------------------------------------------
//  Helper menthod to return a formatted SlimNet address (for display purposes)
//------------------------------------------------------------------------------
   string FormattedSlimNetAdd(int *addr,int level) {
     char address[10]; // 10 levels
     std::sprintf(address,"<%d",addr[0]); 
     for(int i=1;i<level;i++)
	std::sprintf(address,"%s,%d",address,addr[i]);
     std::sprintf(address,"%s>",address);
     return string(address);
}

//------------------------------------------------------------------------------
//  Helper menthod to generate Slimnet address 
//------------------------------------------------------------------------------
void genSlimNetAddress(int number, int X, int *address, int length) {
   int i=0;
   while(number>0)  //number in base 10
   {
     address[length-1-i]= (number%X);
     number = number/X;
     i++;
   }

   while(i<length)
   { 
     address[length-1-i]=0;
     i++;
   }
}

//------------------------------------------------------------------------------
//  Path WSSlimNetAlgorithm::compute()
//------------------------------------------------------------------------------
Path SlimNetAlgorithm::compute(const int &flow_source,
                                const int &flow_destination,
                                Topology* topology)
{
    TRACE("SlimNetAlgorithm::compute -->");
    Path result;

    const int number_of_nodes = topology->getNumNodes();
    struct NodeLabel
    {
        int     predecessor;
        double  length;
        bool    visited;
    };

    struct LevelPath
    {
	int level;
	int source;
	int destination;
    };
    IntVector extras = topology->getExtras();
    int k = extras[0];  // No of levels
    int hosts_per_rack = extras[1];  // Hosts per rack 
    int racks_per_level = extras[2];  // Racks in each level , safely assume 8 
    int tors = pow (racks_per_level, k+1);
    int hosts = hosts_per_rack*tors;
    int addresses[tors][k+1];
    NodeLabel* state = new NodeLabel[tors];  // Maximum number of nodes possible in path
    std::deque<LevelPath> pathq;

    // Node has to be one of the TORs and not hosts, if not return null.
    if(flow_source < hosts || flow_destination < hosts)
	return result;

    // Generate addresses in slimnet format
    for(int node= 0;node < tors; node++) {
          genSlimNetAddress(node,racks_per_level,addresses[node],k+1);
//        cout<<"node="<<node<<"Address="<<FormattedSlimNetAdd(addresses[node],k+1)<<endl;
       }
//
    // Initialization - mark all nodes as not visited
    for (int counter = 0; counter < tors; ++counter)
    {
        // CHANGED FROM 0 TO (N+1) BECAUSE TOPOLOGY STARTS COUNTING AT NODE 0
        state[counter].predecessor = tors + 1;
        state[counter].length = DBL_MAX;
        state[counter].visited  = false;
    }

//    state[src].length = 0.0;
//    state[src].visited = true;
    LevelPath lpath;
     // Push source and destination to the queue
    lpath.source = flow_source - hosts;
    lpath.destination = flow_destination - hosts;
    lpath.level = k;
    pathq.push_back(lpath);

//    cout<<"\nSlimnet(k,hosts_per_rack,racks_per_level): ("<<k<<","<<hosts_per_rack<<","<<racks_per_level<<") , NumNodes="<<number_of_nodes<<endl;

    cout<<"SRC="<<flow_source-hosts<<"["<<FormattedSlimNetAdd(addresses[flow_source-hosts],k+1)<<"] to ";
    cout<<"DEST="<<flow_destination-hosts<<"["<<FormattedSlimNetAdd(addresses[flow_destination-hosts],k+1)<<"]"<<endl;
    for (int level = k; level >= 0; level--)
    {
 	while(!pathq.empty() && pathq.front().level==level) 
       {
	  LevelPath nextpath = pathq.front();
	  pathq.pop_front();
	  int src = nextpath.source;
	  int dest = nextpath.destination;
	  int i_level=addresses[src][k-level], j_level=addresses[dest][k-level]; /* Address stored in opposite order */
	  if(level==0) 
	   {
	     // If connected then direct link
	     if((i_level%2==0 && j_level%2!=0)||(i_level%2!=0 && j_level%2==0))
	       {
	        // Create path between src and dest and mark it as visited
       		state[dest+hosts].predecessor = src + hosts;
       		state[dest+hosts].length = state[src + hosts].length + 1.0; /* default 1 metric */
       		state[dest+hosts].visited = true;
  		cout<<src<<"["<<FormattedSlimNetAdd(addresses[src],k+1)<<"]<<--"<<dest<<"["<<FormattedSlimNetAdd(addresses[dest],k+1)<<"]"<<endl;
	       }
	      else
	       { 
		 int t_level = (j_level + 1) % racks_per_level;
		 int temp = dest - j_level + t_level;
	         // Create paths between src,temp and temp,dest and mark it as visited
       		 state[temp+hosts].predecessor = src + hosts;
       		 state[temp+hosts].length = state[src + hosts].length + 1.0; /* default 1 metric */
       		 state[temp+hosts].visited = true;
  		 cout<<src<<"["<<FormattedSlimNetAdd(addresses[src],k+1)<<"]<<--"<<temp<<"["<<FormattedSlimNetAdd(addresses[temp],k+1)<<"]"<<endl;

       		 state[dest+hosts].predecessor = temp + hosts;
       		 state[dest+hosts].length = state[temp + hosts].length + 1.0; /* default 1 metric */
       		 state[dest+hosts].visited = true;
  		 cout<<temp<<"["<<FormattedSlimNetAdd(addresses[temp],k+1)<<"]<<--"<<dest<<"["<<FormattedSlimNetAdd(addresses[dest],k+1)<<"]"<<endl;
	       }
	     continue;	
	  }

	  if(i_level == j_level) // No need to do anything, just go to next level
	     continue;
	  else
	   {
	     int i_0 = addresses[src][k]; 
	     // Calculate Fxy to see if there is a direct link betweek i_level and j_level
             int Fxy = (i_0==0) ? ((racks_per_level-1)-i_level) : ((i_level|i_0)&((racks_per_level-1)-(i_level&i_0)));

	     // Direct link exists!
	     if(Fxy == j_level) {
		int temp = i_0, mul=1;
		for(int l=1;l<level;l++) {
		   mul*=racks_per_level;
		   temp+=(mul*addresses[src][k-l]); /* address stored in opposite order */
		}
		for(int l=level;l<=k;l++) {
		   mul*=racks_per_level;
		   temp+=(mul*addresses[dest][k-l]);  /* address stored in opposite order */
		}
	        // Create path between src and temp and mark it as visited
       		state[temp+hosts].predecessor = src + hosts;
       		state[temp+hosts].length = state[src + hosts].length + 1.0; /* default 1 metric */
       		state[temp+hosts].visited = true;
  		 cout<<src<<"["<<FormattedSlimNetAdd(addresses[src],k+1)<<"]<<--"<<temp<<"["<<FormattedSlimNetAdd(addresses[temp],k+1)<<"]"<<endl;
		// Remaining (temp,dest,l-1) - insert to pathq
		LevelPath temppath;
		temppath.source=temp;
		temppath.destination=dest;
		temppath.level=level-1;
		pathq.push_back(temppath);
	     }
	     else // Direct link doesnot exist, find temp1 and temp2
	     {
             	Fxy = (j_level==0) ? ((racks_per_level-1)-i_level) : ((i_level|j_level)&((racks_per_level-1)-(i_level&j_level)));
		int temp = Fxy, mul=1;
		for(int l=1;l<level;l++) {
		   mul*=racks_per_level;
		   temp+=(mul*addresses[src][k-l]);  /* addresses stored in opposite order */
		}
		mul*=racks_per_level;
		int temp1=temp+(mul*addresses[src][k-level]); /* opposite order */
		int temp2=temp+(mul*addresses[dest][k-level]); 
		for(int l=level+1;l<=k;l++) {
		   mul*=racks_per_level;
		   temp1+=(mul*addresses[dest][k-l]); /* addresses stored in opposite order */
		   temp2+=(mul*addresses[dest][k-l]);
		}
	        // Create path between temp1 and temp2 and mark it as visited
       		state[temp2+hosts].predecessor = temp1 + hosts;
       		state[temp2+hosts].length = state[temp1 + hosts].length + 1.0; /* Assume default 1 metric */
       		state[temp2+hosts].visited = true;
  		 cout<<temp1<<"["<<FormattedSlimNetAdd(addresses[temp1],k+1)<<"]<<--"<<temp2<<"["<<FormattedSlimNetAdd(addresses[temp2],k+1)<<"]"<<endl;
		// Remaining is (src,temp1,0) and (temp2,dest,l-1) - insert to pathq
		LevelPath temppath1, temppath2;
		temppath1.source=src;
		temppath1.destination=temp1;
		temppath1.level=0;

		temppath2.source=temp2;
		temppath2.destination=dest;
		temppath2.level=level-1;
		pathq.push_back(temppath1);
		pathq.push_back(temppath2);
	     }
 	}
    }
  }
    // path calculation
    int worknode;
    cout<<"Path from :: ";
    cout<<"src="<<flow_source-hosts<<"["<<FormattedSlimNetAdd(addresses[flow_source-hosts],k+1)<<"] to ";
    cout<<"dest="<<flow_destination-hosts<<"["<<FormattedSlimNetAdd(addresses[flow_destination-hosts],k+1)<<"]"<<endl;
    if (state[flow_destination].visited) // path was found
    {
        worknode = flow_destination;
        while (worknode != flow_source)
        {
            // calculates path in reverse order
	    cout<<worknode-hosts<<"["<<FormattedSlimNetAdd(addresses[worknode-hosts],k+1)<<"]<<---";
            result.push_front(worknode);
            worknode = state[worknode].predecessor;
        }
	cout<<worknode-hosts<<"["<<FormattedSlimNetAdd(addresses[worknode-hosts],k+1)<<"]"<<endl;
        result.push_front(flow_source);
    }

    delete [] state;

    TRACE("SlimNetAlgorithm::compute <--");
    return result;
 }

