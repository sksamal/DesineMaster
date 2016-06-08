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

// Print SlimNet Address 
   string FormattedSlimNetAdd(int *addr,int level) {
     char address[10]; // 10 levels
     std::sprintf(address,"<%d",addr[0]); 
     for(int i=1;i<level;i++)
	std::sprintf(address,"%s,%d",address,addr[i]);
     std::sprintf(address,"%s>",address);
     return string(address);
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
    NodeLabel* state = new NodeLabel[number_of_nodes];
    IntVector extras = topology->getExtras();
    int k = extras[0];  // No of levels
    int hosts_per_rack = extras[1];  // Hosts per rack 
    int racks_per_level = extras[2];  // Range of y, No of cells = x*y

    int hosts = hosts_per_rack*aggs;
    int addresses[hosts][k+1];
    for(int node= host;node < hosts; node++) {
          genSlimNetAddress(node,racks_per_level,addresses[node],k+1);
//        cout<<"node="<<node<<"Address="<<FormattedSlimNetAdd(addresses[node],k+1)<<endl;
       }
//
    // Initialization
    for (int counter = 0; counter < number_of_nodes; ++counter)
    {
        // CHANGED FROM 0 TO (N+1) BECAUSE TOPOLOGY STARTS COUNTING AT NODE 0
        state[counter].predecessor = number_of_nodes + 1;
        state[counter].length = DBL_MAX;
        state[counter].visited  = false;
    }
    int worknode = flow_source; // First worknode is the source
    int dest = flow_destination; // First worknode is the source
    state[worknode].length = 0.0;
    state[worknode].visited = true;
    int worknode = address[flow_source];
    int dest = address[flow_destination];

    cout<<"\nSlimnet(k,hosts_per_rack,racks_per_level): ("<<k<<","<<hosts_per_rack<<","<<racks_per_level<<") , NumNodes="<<number_of_nodes<<endl;

    updateRecursive(flow_source,flow_destination,k,state);
    if(!state[flow_destination].visited)
    {  // Some problem
	PRINT("Error");
	return NULL;
    } // end: while...

    // path calculation
    if (state[flow_destination].visited) // path was found
    {
        worknode = flow_destination;
        while (worknode != flow_source)
        {
            result.push_front(worknode);
            worknode = state[worknode].predecessor;
        }
        result.push_front(flow_source);
    }

    delete [] state;

    TRACE("SlimNetAlgorithm::compute <--");
    return result;
}

void updateRecursive(int src, int dest, int l, int *state) 
  {
	if(src == dest) break;
	else
	{
	  if(l==0) {  // only one level exists here
		// If worknode and dest are connected, create the path and return.
	  }
	  else {
	        // Connect using the next node, which has to be connected.  
  	  }	        
	}
       state[destination].predecessor = worknode + tor;
       state[destination].length = state[worknode+ tor].length + 1.0; /* Assume default 1 metric */
       state[destination].visited = true;
       worknode = destination - tor;
  } 
