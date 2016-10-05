/******************************************************************************

    source file FatTreeAlgorithm.cpp for class: FatTreeAlgorithm

    See header file for details

    Author : Suraj Ketan Samal
    Version: 1
    Date   : October 5, 2016 
    Updated: Created this to implement a deterministic routing for fattree 
******************************************************************************/


// Include(s)
#include "Algorithms/Algorithm.h"
#include "Algorithms/FatTreeAlgorithm.h"
#include "Network/Flow.h"
#include "Network/LinkList.h"
#include "Network/Path.h"
#include "Network/Topology.h"
#include "Utils/TraceManager.h"
#include "RandomVariables/RandomNumberGenerator.h"
#include "RandomVariables/RandomVar.h"
#include "RandomVariables/UniformVar.h"
#include <cfloat>
#include <cmath>
#include <vector>
using std::vector;

// Constanst(s)

// Variable(s)

// Function(s) definitions

//------------------------------------------------------------------------------
//  constructor FatTreeAlgorithm::FatTreeAlgorithm()
//------------------------------------------------------------------------------
FatTreeAlgorithm::FatTreeAlgorithm(const TString &args) :
                   Algorithm(args)
{
    TRACE("FatTreeAlgorithm::FatTreeAlgorithm -->");
    rng = RandomNumberGenerator::getRandomNumberGenerator();
    TRACE("FatTreeAlgorithm::FatTreeAlgorithm <--");
}


//------------------------------------------------------------------------------
//  destructor FatTreeAlgorithm::~FatTreeAlgorithm()
//------------------------------------------------------------------------------
FatTreeAlgorithm::~FatTreeAlgorithm()
{
    TRACE("FatTreeAlgorithm::~FatTreeAlgorithm -->");
    // Empty
    TRACE("FatTreeAlgorithm::~FatTreeAlgorithm <--");
}

//------------------------------------------------------------------------------
//  Helper menthod to return a formatted FatTree address (for display purposes)
//------------------------------------------------------------------------------
   string FormattedFatTreeAdd(int *addr,int level) {
     char address[20]; // 10 levels
     std::sprintf(address,"<%d",addr[0]); 
     for(int i=1;i<level;i++)
	std::sprintf(address,"%s,%d",address,addr[i]);
     std::sprintf(address,"%s>",address);
     return string(address);
}

/*
//------------------------------------------------------------------------------
//  Helper menthod to generate Fattree address 
//------------------------------------------------------------------------------
void genFatTreeAddress(int number, int X, int *address, int length) {
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
} */

//------------------------------------------------------------------------------
//  Helper menthod to generate FatTree address 
//------------------------------------------------------------------------------
void genFatTreeAddress(int number, int X, int *address, int length) {
   for(int i=length-1;i>0;i--)
   {
     address[i]= (number%X);
     number = number/X;
     if(number<0)
     	address[i]=0;
   }
   address[0] = number;    
}

//------------------------------------------------------------------------------
//  Path FatTreeAlgorithm::compute()
//------------------------------------------------------------------------------
Path FatTreeAlgorithm::compute(const int &flow_source,
                                const int &flow_destination,
                                Topology* topology)
{
    TRACE("FatTreeAlgorithm::compute -->");
    Path result;

    const int number_of_nodes = topology->getNumNodes();
    struct NodeLabel
    {
        int     predecessor;
        double  length;
        bool    visited;
    };

    IntVector extras = topology->getExtras();
    int k = extras[0];  // Value of k
    int hosts = k*k*k/4, host=0;
    int edges = k*k/2, edge=hosts;
    int aggs  = k*k/2, agg=edge+edges;
    int cores = k*k/4, core=agg+aggs;
    int addresses[number_of_nodes][5];
//    for( int i=0; i<number_of_nodes; i++)
// 	addresses[i] = new int [5];
    NodeLabel* state = new NodeLabel[number_of_nodes];  // Maximum number of nodes possible in path

    // Node has to be one of the hosts and not TORs, if not return null.
    if(flow_source > hosts || flow_destination > hosts)
	return result;

    // Generate addresses in format
    for(int node= 0;node < hosts; node++) {
          genFatTreeAddress(node,k/2,addresses[node],5);
//        cout<<"node="<<node<<"Address="<<FormattedFatTreeAdd(addresses[node],5)<<endl;
       }
    // Generate addresses in format
    for(int node= 0;node < edges; node++) {
          genFatTreeAddress(k*k*k*k/16+node,k/2,addresses[edge+node],5);
  //      cout<<"edge="<<edge+node<<"Address="<<FormattedFatTreeAdd(addresses[edge+node],5)<<endl;
       }
    // Generate addresses in format
    for(int node= 0;node < aggs; node++) {
          genFatTreeAddress(2*k*k*k*k/16+node,k/2,addresses[agg+node],5);
  //      cout<<"agg="<<agg+node<<"Address="<<FormattedFatTreeAdd(addresses[agg+node],5)<<endl;
       }
    for(int node= 0;node < cores; node++) {
          genFatTreeAddress(3*k*k*k*k/16+node,k/2,addresses[core+node],5);
  //      cout<<"core="<<core+node<<"Address="<<FormattedFatTreeAdd(addresses[core+node],5)<<endl;
       }
//
    // Initialization - mark all nodes as not visited
    for (int counter = 0; counter < 8; ++counter)
    {
        // CHANGED FROM 0 TO (N+1) BECAUSE TOPOLOGY STARTS COUNTING AT NODE 0
        state[counter].predecessor = 9;
        state[counter].length = DBL_MAX;
        state[counter].visited  = false;
    }

    int src = flow_source;   /* Host to host routing in this topology */
    int dest = flow_destination;
    int worknode = src;
    int workaddr[5];
    RandomNumberGenerator *rng = RandomNumberGenerator::getRandomNumberGenerator();
    RandomVar *uvar = (RandomVar*) new UniformVar(rng, 0, (k/2));
    for(int l=0;l<5;l++)
 	workaddr[l] = addresses[worknode][l];
//    cout<<"\nFatTree(k): ("<<k<<") , NumNodes="<<number_of_nodes<<endl;

//    cout<<"SRC="<<flow_source<<"["<<FormattedFatTreeAdd(addresses[flow_source],5)<<"] to ";
//    cout<<"DEST="<<flow_destination<<"["<<FormattedFatTreeAdd(addresses[flow_destination],5)<<"]"<<endl;
	int tempaddr[5];	

	// Find next edge
	tempaddr[0] = 1;
	tempaddr[1] = 0;
	for(int l=1;l<5;l++) //Address in opposite order 
	  tempaddr[l+1] = workaddr[l];
	int mul=1;
	int tempnode=0;
	for(int l=0;l<4;l++) {
	   tempnode+=(mul*tempaddr[4-l]); /* address stored in opposite order */
	   mul*=(k/2);
	}
       	state[tempnode+edge].predecessor = worknode;
       	state[tempnode+edge].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[tempnode+edge].visited = true;
// 	cout<<worknode<<"["<<FormattedFatTreeAdd(workaddr,5)<<"][Host]-->>"<<tempnode+edge<<"("<<tempnode<<") ["<<FormattedFatTreeAdd(addresses[tempnode+edge],5)<<"][Edge]"<<endl;
	worknode = tempnode + edge;	
    	for(int l=0;l<5;l++)
 	   workaddr[l] = tempaddr[l];

    	int *destaddr = addresses[dest];
	if( workaddr[2] != destaddr[1] || workaddr[3] != destaddr[2] || workaddr[4] != destaddr[3]) {
	// Find next agg
	tempaddr[0] = 2;
        double var = uvar->generate();
//        cout<<"URand="<<var<<"("<<(int)(var)<<")"<<endl;
	tempaddr[4] = (int)(var);//(k/2)*std::rand()/RAND_MAX;
	mul=1;
	tempnode=0;
	for(int l=0;l<4;l++) {
	   tempnode+=(mul*tempaddr[4-l]); /* address stored in opposite order */
	   mul*=(k/2);
	}
       	state[tempnode+agg].predecessor = worknode;
       	state[tempnode+agg].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[tempnode+agg].visited = true;
 //	cout<<worknode<<"("<<worknode - edge<<")["<<FormattedFatTreeAdd(workaddr,5)<<"][Edge]-->>"<<tempnode+agg<<"("<<tempnode<<") ["<<FormattedFatTreeAdd(addresses[tempnode+agg],5)<<"][Agg]"<<endl;
	worknode = tempnode + agg;
    	for(int l=0;l<5;l++)
 	   workaddr[l] = tempaddr[l];

	if( workaddr[2] != destaddr[1] || workaddr[3] != destaddr[2]) {
	// Find next core
	tempaddr[0] = 3;
	tempaddr[1] = tempaddr[2] = 0;
	tempaddr[3] = workaddr[4];
        double var = uvar->generate();
  //      cout<<"CRand="<<var<<"("<<(int)(var)<<")"<<endl;
	tempaddr[4] = (int)(var);//(k/2)*std::rand()/RAND_MAX;
	mul=1;
	tempnode=0;
	for(int l=0;l<4;l++) {
	   tempnode+=(mul*tempaddr[4-l]); /* address stored in opposite order */
	   mul*=(k/2);
	}
       	state[tempnode+core].predecessor = worknode;
       	state[tempnode+core].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[tempnode+core].visited = true;
 //	cout<<worknode<<"("<<worknode-agg<<")["<<FormattedFatTreeAdd(workaddr,5)<<"][Agg]-->>"<<tempnode+core<<"("<<tempnode<<") ["<<FormattedFatTreeAdd(addresses[tempnode+core],5)<<"][Core]"<<endl;
	worknode = tempnode + core;
    	for(int l=0;l<5;l++)
 	   workaddr[l] = tempaddr[l];

	// Find next agg
	tempaddr[0] = 2;
	tempaddr[1] = 0;
	tempaddr[2] = destaddr[1];
	tempaddr[3] = destaddr[2];
	tempaddr[4] = workaddr[3];
	mul=1;
	tempnode=0;
	for(int l=0;l<4;l++) {
	   tempnode+=(mul*tempaddr[4-l]); /* address stored in opposite order */
	   mul*=(k/2);
	}
       	state[tempnode+agg].predecessor = worknode;
       	state[tempnode+agg].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[tempnode+agg].visited = true;
 //	cout<<worknode<<"("<<worknode-core<<")["<<FormattedFatTreeAdd(workaddr,5)<<"][Core]-->>"<<tempnode+agg<<"("<<tempnode<<") ["<<FormattedFatTreeAdd(addresses[tempnode+agg],5)<<"][Agg]"<<endl;
	worknode = tempnode + agg;
    	for(int l=0;l<5;l++)
 	   workaddr[l] = tempaddr[l];
	}
	
	// Find next edge
	tempaddr[0] = 1;
	tempaddr[1] = 0;
	tempaddr[2] = destaddr[1];
	tempaddr[3] = destaddr[2];
	tempaddr[4] = destaddr[3];
	mul=1;
	tempnode=0;
	for(int l=0;l<4;l++) {
	   tempnode+=(mul*tempaddr[4-l]); /* address stored in opposite order */
	   mul*=(k/2);
	}
       	state[tempnode+edge].predecessor = worknode;
       	state[tempnode+edge].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[tempnode+edge].visited = true;
 //	cout<<worknode<<"("<<worknode-agg<<")["<<FormattedFatTreeAdd(workaddr,5)<<"][Agg]-->>"<<tempnode+edge<<"("<<tempnode<<") ["<<FormattedFatTreeAdd(addresses[tempnode+edge],5)<<"][Edge]"<<endl;
	worknode = tempnode + edge;
    	for(int l=0;l<5;l++)
 	   workaddr[l] = tempaddr[l];
	}

	// Destination node
       	state[dest].predecessor = worknode;
       	state[dest].length = state[worknode].length + 1.0; /* default 1 metric */
       	state[dest].visited = true;
 //	cout<<worknode<<"("<<worknode-edge<<")["<<FormattedFatTreeAdd(workaddr,5)<<"][Edge]-->>"<<dest<<"["<<FormattedFatTreeAdd(addresses[dest],5)<<"][Host]"<<endl;

    // path calculation
//    cout<<"Path from :: ";
//    cout<<"src="<<flow_source<<"["<<FormattedFatTreeAdd(addresses[flow_source],k+1)<<"] to ";
//    cout<<"dest="<<flow_destination<<"["<<FormattedFatTreeAdd(addresses[flow_destination],k+1)<<"]"<<endl;
    if (state[flow_destination].visited) // path was found
    {
        worknode = flow_destination;
        while (worknode != flow_source)
        {
            // calculates path in reverse order
//	    cout<<worknode<<"["<<FormattedFatTreeAdd(addresses[worknode],k+1)<<"]<<---";
            result.push_front(worknode);
            worknode = state[worknode].predecessor;
            
            Link* lk = topology->link(result[0], worknode);

       	if(lk == NULL) {
	   cout<<"Link not found between "<<result[0]<< " and " <<worknode<<endl;
	}
        }
//	cout<<worknode<<"["<<FormattedFatTreeAdd(addresses[worknode],k+1)<<"]"<<endl;
        result.push_front(flow_source);
    }
     
    if(result.size()==0) {
    cout<<"Blocked: src="<<flow_source<<"["<<FormattedFatTreeAdd(addresses[flow_source],k+1)<<"] to ";
    cout<<"dest="<<flow_destination<<"["<<FormattedFatTreeAdd(addresses[flow_destination],k+1)<<"]"<<endl;
    }
    delete [] state;
//    for( int i=0; i<number_of_nodes; i++)
// 	delete [] addresses[i];
    TRACE("FatTreeAlgorithm::compute <--");
    return result;
 }

