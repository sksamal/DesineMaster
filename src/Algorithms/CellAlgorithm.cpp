/******************************************************************************

    source file CellAlgorithm.cpp for class: CellAlgorithm

    See header file for details. Adapted from other algorithms
    Suraj May,2016

******************************************************************************/



// Include(s)
#include "Algorithms/Algorithm.h"
#include "Algorithms/CellAlgorithm.h"
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
//  constructor CellAlgorithm::CellAlgorithm()
//------------------------------------------------------------------------------
CellAlgorithm::CellAlgorithm(const TString &args) :
                   Algorithm(args)
{
    TRACE("CellAlgorithm::CellAlgorithm -->");
    rng = RandomNumberGenerator::getRandomNumberGenerator();
    TRACE("CellAlgorithm::CellAlgorithm <--");
}


//------------------------------------------------------------------------------
//  destructor CellAlgorithm::~CellAlgorithm()
//------------------------------------------------------------------------------
CellAlgorithm::~CellAlgorithm()
{
    TRACE("CellAlgorithm::~CellAlgorithm -->");
    // Empty
    TRACE("CellAlgorithm::~CellAlgorithm <--");
}


//------------------------------------------------------------------------------
//  Path WSCellAlgorithm::compute()
//------------------------------------------------------------------------------
Path CellAlgorithm::compute(const int &flow_source,
                                const int &flow_destination,
                                Topology* topology)
{
    TRACE("CellAlgorithm::compute -->");
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
    int k = extras[0];  // No of servers in a rack
    int x = extras[1];  // Range of X = 0 to 2*x 
    int y = extras[2];  // Range of Y= 0 to 2*y
    int tor =k*(-x+y+2*x*y-1);
   // cout<<"\nCell(k,x,y): ("<<k<<","<<x<<","<<y<<") , NumNodes="<<number_of_nodes<<endl;

    // Node has to be one of the TORs
    if(flow_source < tor || flow_destination < tor)
	return result;

    // Initialization
    for (int counter = 0; counter < number_of_nodes; ++counter)
    {
        // CHANGED FROM 0 TO (N+1) BECAUSE TOPOLOGY STARTS COUNTING AT NODE 0
        state[counter].predecessor = number_of_nodes + 1;
        state[counter].length = DBL_MAX;
        state[counter].visited  = false;
    }

    int worknode, src, dest;
    worknode = src = flow_source - tor; // First worknode is the source
    dest = flow_destination - tor; // Destination
    state[worknode+tor].length = 0.0;
    state[worknode+tor].visited = true;
    int work_x = worknode;
    int work_y = 0,cx=x;
    while (work_x >= cx) {
      work_x = work_x - cx;
      if(work_y%2==0) 
	cx = x+1;	
      else
	cx = x;
      work_y++;
    }

    int dest_x = dest;
    int dest_y = 0;
    cx=x;
    while (dest_x >=cx) {
      dest_x = dest_x - cx;
      if(dest_y%2==0) 
	cx = x+1;
      else
	cx = x;
      dest_y++;
    }

   // cout<<"src="<<src<<" ("<<work_x<<","<<work_y<<") to ";
   // cout<<"dest="<<dest<<" ("<<dest_x<<","<<dest_y<<")"<<endl;
    // Repeat until destination has been visited
    while(!state[flow_destination].visited)
    {
       int delta_x = dest_x - work_x;
       int delta_y = dest_y - work_y;
//       cout<<"dx="<<delta_x<<" dy="<<delta_y<<" Flow destination:"<<flow_destination<<endl;

       if(work_y%2 == 0)   /* Even Y */
        {
	   if(delta_x > 0 && delta_y > 0 && delta_x >= delta_y) { //Port 1 
		work_x++;
		work_y++;
	   }
	   //else if (delta_x<=0 && delta_y>1) 		//Port 2
	   else if (delta_y>1) 		//Port 2
		work_y+=2;
       	   else if(delta_x<=0 && delta_y>0)   //Port 3
		work_y++;
	   //else if(delta_x<=0 && delta_y<-1)  //Port 5
	   else if(delta_y<-1)  //Port 5
		work_y-=2;
           else if (delta_x<=0 && delta_y<0) 		//Port 4
		work_y--;
	   else if (delta_x > 0 && delta_y<0 && delta_x>=-delta_y) {  //Port 6
		work_x++;
		work_y--;
	   }
	   else	if(delta_y==0 && delta_x > 0) {
	         if(work_y ==0 ) // lower row
		  { work_x++; work_y++; }  // Port 1
	        else if(work_y == (2*y)) // uppermost row
		  { work_x++; work_y--; }   // Port 6 
		else {  /* Use either one */
		  double rnum = std::rand()/RAND_MAX;
		  if(rnum>0.5) { work_x++; work_y++; }  /* Port 1 */
		  else	     { work_x++; work_y--; }  /* Port 6 */
		}
		
	   }
	   else if(delta_y==0 && delta_x < 0) {
		 if(work_y==0) // lower row
		  work_y++; 
	         else if(work_y == (2*y)) // uppermost row
		  work_y--; 
		else {  /* Use either one */
		  double rnum = std::rand()/RAND_MAX;
		  if(rnum>0.5) { work_y++; }
		  else	     { work_y--; }
		}
	   }
	}
       else		  /* Odd Y*/
        {
           if (delta_x>0 && delta_y<0 && delta_x<=-delta_y) {
		work_y--;	// Port 1	
	   }
	   else if ((delta_x>0 && delta_y==0) 
		|| (delta_x>0 && delta_y<0 && delta_x>-delta_y) 
		|| (delta_x>0 && delta_y>0 && delta_x>delta_y)) 
		work_x++;		// Port 2
	   else if (delta_x>0 && delta_y>0 && delta_x<=delta_y) {
		work_y++;		
	   }
	   else if (delta_x<0 && delta_y>0 && -delta_x<=delta_y) {
		work_x--;		// Port 4
		work_y++;
	   }
	   else if ((delta_x<0 && delta_y==0) 
		|| (delta_x<0 && delta_y>0 && -delta_x>delta_y) 
		|| (delta_x<0 && delta_y<0 && -delta_x>-delta_y))
		work_x--;		// Port 5
	   else if (delta_x<0 && delta_y<0 && -delta_x <=-delta_y) {
		work_x--;		// Port 6
		work_y--;
	   }
	   else	if(delta_x ==0 && delta_y > 0) {
		 if(work_x==0 || delta_y==1)  work_y++;	 
		 else if(work_x== (2*x)) 
		  { work_y++; work_x--;	}
		 else {
		  double rnum = std::rand()/RAND_MAX;
		  if(rnum>0.5) { work_y++; }
		  else	     { work_x--; work_y++; }
		}
	  }
	  else if(delta_x ==0 && delta_y < 0) {
		if(work_x==0 || delta_y==-1) work_y--;
		else if(work_x== (2*x))
		{ work_y--;work_x--; }
		else {
		double rnum = std::rand()/RAND_MAX;
                  if(rnum>0.5) { work_y--; }
                  else       { work_x--; work_y--; }
                }
	   }
        } 
  //     cout<<"WorkX="<<work_x<<" , WorkY="<<work_y<<endl; 
       int destination = work_y*x + (work_y/2) + work_x + tor;	
  //     cout<<"Work Node:"<<worknode<<" Next Node: "<< work_y*x + work_x  + (work_y/2) << "("<<work_x<<","<<work_y<<")"<<endl;
  //     cout<<destination<<"("<<destination-tor<<")<<--"<<worknode+tor<<"("<<worknode<<")"<<endl;
       state[destination].predecessor = worknode + tor;
       state[destination].length = state[worknode+ tor].length + 1.0; /* Assume default 1 metric */
       state[destination].visited = true;
       worknode = destination - tor;
    } // end: while...

    // path calculation
   // cout<<"Path:: ";
    if (state[flow_destination].visited) // path was found
    {
        worknode = flow_destination;
        while (worknode != flow_source)
        {
            // add node at front because dijktra
            // calculates path in reverse order
	   // cout<<worknode<<"("<<worknode-tor<<")<<---";
            result.push_front(worknode);
            worknode = state[worknode].predecessor;
        }
//	cout<<worknode<<"("<<worknode-tor<<")"<<endl;
        result.push_front(worknode);
    }
    delete [] state;
    TRACE("CellAlgorithm::compute <--");
    return result;
}
