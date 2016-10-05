/******************************************************************************

    source file BCubeAlgorithm.cpp for class: BCubeAlgorithm

    See header file for details

    Author : Suraj Ketan Samal
    Version: 1
    Date   : Sep 15, 2016 
    Updated: Removed the level for loop to remove the restriction, changed the 
             order of pushing temp1 and temp2
    08/14/2016: - Suraj - cleanup debug messages
******************************************************************************/


// Include(s)
#include "Algorithms/Algorithm.h"
#include "Algorithms/BCubeAlgorithm.h"
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
//  constructor BCubeAlgorithm::BCubeAlgorithm()
//------------------------------------------------------------------------------
BCubeAlgorithm::BCubeAlgorithm(const TString &args) :
                   Algorithm(args)
{
    TRACE("BCubeAlgorithm::BCubeAlgorithm -->");
    rng = RandomNumberGenerator::getRandomNumberGenerator();
    TRACE("BCubeAlgorithm::BCubeAlgorithm <--");
}


//------------------------------------------------------------------------------
//  destructor BCubeAlgorithm::~BCubeAlgorithm()
//------------------------------------------------------------------------------
BCubeAlgorithm::~BCubeAlgorithm()
{
    TRACE("BCubeAlgorithm::~BCubeAlgorithm -->");
    // Empty
    TRACE("BCubeAlgorithm::~BCubeAlgorithm <--");
}

//------------------------------------------------------------------------------
//  Helper menthod to return a formatted BCube address (for display purposes)
//------------------------------------------------------------------------------
   string FormattedBCubeAdd(int *addr,int level) {
     char address[10]; // 10 levels
     std::sprintf(address,"<%d",addr[0]); 
     for(int i=1;i<level;i++)
	std::sprintf(address,"%s,%d",address,addr[i]);
     std::sprintf(address,"%s>",address);
     return string(address);
}

//------------------------------------------------------------------------------
//  Helper menthod to generate BCube address 
//------------------------------------------------------------------------------
void genBCubeAddress(int number, int X, int *address, int length) {
   int i=0;
   for(int i=length-1;i>0;i--)
   {
     address[i]= (number%X);
     number = number/X;
     if(number<0)
     	address[i]=0;
   }
   address[0] = number;

/*
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
   } */
}

//------------------------------------------------------------------------------
//  Path BCubeAlgorithm::compute()
//------------------------------------------------------------------------------
Path BCubeAlgorithm::compute(const int &flow_source,
                                const int &flow_destination,
                                Topology* topology)
{
    TRACE("BCubeAlgorithm::compute -->");
    Path result;

    const int number_of_nodes = topology->getNumNodes();
    struct NodeLabel
    {
        int     predecessor;
        double  length;
        bool    visited;
    };

    IntVector extras = topology->getExtras();
    int k = extras[0];  // No of levels
    int n = extras[1];  // Hosts per rack 
    int tors = (k+1) * pow (n, k);
    int hosts = pow (n, k+1);
    int addresses[tors][k+1];
    int hostaddr[hosts][k+1];
    NodeLabel* state = new NodeLabel[hosts+tors];  // Maximum number of nodes possible in path

    // Node has to be one of the hosts and not TORs, if not return null.
    if(flow_source > hosts || flow_destination > hosts)
	return result;

    // Generate addresses in slimnet format
    for(int node= 0;node < tors; node++) {
          genBCubeAddress(node,n,addresses[node],k+1);
    //    cout<<"node="<<node<<"Address="<<FormattedBCubeAdd(addresses[node],k+1)<<endl;
       }
    // Generate addresses in slimnet format
    for(int node= 0;node < hosts; node++) {
          genBCubeAddress(node,n,hostaddr[node],k+1);
//        cout<<"host="<<node<<"Address="<<FormattedBCubeAdd(hostaddr[node],k+1)<<endl;
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

    int src = flow_source;   /* Host to host routing in this topology */
    int dest = flow_destination;
    int worknode = src;
    int workaddr[k+1];
    for(int l=0;l<k+1;l++)
	workaddr[l] = hostaddr[worknode][l];
 //   cout<<"\nBCube(k,n): ("<<k<<","<<n<<") , NumNodes="<<number_of_nodes<<endl;

//    cout<<"SRC="<<flow_source<<"["<<FormattedBCubeAdd(hostaddr[flow_source],k+1)<<"] to ";
//    cout<<"DEST="<<flow_destination<<"["<<FormattedBCubeAdd(hostaddr[flow_destination],k+1)<<"]"<<endl;
	
	for(int level = k; level >=0; level--)
       {
	  if(hostaddr[src][k-level] != hostaddr[dest][k-level])
	   {
		// Find the appropriate next tor
	  	int tempaddr[k+1];	
		tempaddr[0] = level;
		for(int l=1;l<=k-level;l++)  /* Address in opposite order */
		   tempaddr[l] = workaddr[l-1]; 
		for(int l=k-level+1;l<k+1;l++)
		   tempaddr[l] = workaddr[l];
		int temptor=0, mul = 1;
		for(int l=0;l<k+1;l++) {
		   temptor+=(mul*tempaddr[k-l]); /* address stored in opposite order */
		   mul*=n;
		}
	        // Create path between worknode and temptor and mark it as visited
       		state[temptor+hosts].predecessor = worknode;
       		state[temptor+hosts].length = state[worknode].length + 1.0; /* default 1 metric */
       		state[temptor+hosts].visited = true;
 // 		cout<<worknode<<"["<<FormattedBCubeAdd(hostaddr[worknode],k+1)<<"]-->>"<<temptor+hosts<<"("<<temptor<<") ["<<FormattedBCubeAdd(addresses[temptor],k+1)<<"]"<<endl;

		// Find the next host
		workaddr[k-level]=hostaddr[dest][k-level];
		mul=1;
		int tempnode=0;
		for(int l=0;l<k+1;l++) {
		   tempnode+=(mul*workaddr[k-l]); /* address stored in opposite order */
		   mul*=n;
		}
	        // Create path between worknode and tempnode and mark it as visited
       		state[tempnode].predecessor = temptor+hosts;
       		state[tempnode].length = state[temptor+hosts].length + 1.0; /* default 1 metric */
       		state[tempnode].visited = true;
  //		cout<<temptor+hosts<<"("<<temptor<<") ["<<FormattedBCubeAdd(addresses[temptor],k+1)<<"]-->>"<<tempnode<<"["<<FormattedBCubeAdd(hostaddr[tempnode],k+1)<<"]"<<endl;
		worknode = tempnode;
	  }
 	}

    // path calculation
//    cout<<"Path from :: ";
//    cout<<"src="<<flow_source<<"["<<FormattedBCubeAdd(hostaddr[flow_source],k+1)<<"] to ";
//    cout<<"dest="<<flow_destination<<"["<<FormattedBCubeAdd(hostaddr[flow_destination],k+1)<<"]"<<endl;
    if (state[flow_destination].visited) // path was found
    {
        worknode = flow_destination;
        while (worknode != flow_source)
        {
            // calculates path in reverse order
//	    cout<<worknode<<"["<<FormattedBCubeAdd(hostaddr[worknode],k+1)<<"]<<---";
            result.push_front(worknode);
            worknode = state[worknode].predecessor;
            
            Link* lk = topology->link(result[0], worknode);

       	if(lk == NULL) {
	   cout<<"Link not found between "<<result[0]<< " and " <<worknode<<endl;
	}
        }
//	cout<<worknode<<"["<<FormattedBCubeAdd(hostaddr[worknode],k+1)<<"]"<<endl;
        result.push_front(flow_source);
    }
     
    if(result.size()==0) {
    cout<<"Blocked: src="<<flow_source<<"["<<FormattedBCubeAdd(hostaddr[flow_source],k+1)<<"] to ";
    cout<<"dest="<<flow_destination<<"["<<FormattedBCubeAdd(hostaddr[flow_destination],k+1)<<"]"<<endl;
    }
    delete [] state;

    TRACE("BCubeAlgorithm::compute <--");
    return result;
 }

