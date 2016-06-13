/******************************************************************************

    source file TopologyFactoryCell.cpp with implementation of
	createTopologyCell
    Author: Suraj May2016


******************************************************************************/

//------------------------------------------------------------------------------
// Create a Cell graph with k levels of nodes
//
// createTopologyCell()
//------------------------------------------------------------------------------
Topology* TopologyFactory::createTopologyCell(const TString &description)
{
    TRACE("TopologyFactory::createTopologyCell -->");

    Topology* result = new Topology();

    int k = atoi(description.at(1).c_str()); // No of servers in a rack
    int x = atoi(description.at(2).c_str()); // Range of x
    int y = atoi(description.at(3).c_str()); // Range of y, No of cells = x*y
	
	 result->extras.clear();		
    result->extras.push_back(k);		
    result->extras.push_back(x);		
    result->extras.push_back(y);
    result->number_of_nodes   = (-x + y + 2*x*y - 1)*(k+1); // Total Nodes in 
    
    // We label the hosts from 0 to k*(x+y+ 2*x*y) -1
    int host=0, hosts=k*(x+y+2*x*y);

    // We label the TOR nodes from k*x+y+2*x*y) to (k+1)*((x+y+ 2*x*y) -1
    int tor=k*(-x+y+2*x*y-1), tors=(-x+y+2*x*y-1);
    result->first_node = tor;
    result->last_node = tor+tors-1;

    //cout<<"Hosts="<<hosts<<"Tors="<<tors<<endl;
    //cout<<"Total nodes="<<result->number_of_nodes<<endl;

    result->is_directed       = description.size()<5 ? false :
        (atoi(description.at(4).c_str()) != 0);
    result->number_of_qos     = 0;
    result->link_list         = new LinkList(6*result->number_of_nodes);

    if (!(last_description == description))
    {

    last_description = description;
    // Make list of all posible nodepairs 
    pairs.clear();
    edge_nodes.clear();

    
    // We label the TOR nodes from k*x+y+2*x*y) to (k+1)*((x+y+ 2*x*y) -1
    // x --> horizontal direction, y --> vertical direction 
    //  ______________________________________________
    // |   tor+0  tor+1  tor+2..tor+x-1	              |
    // | tor+x   tor+(x+1) ....    tor+2x	      |
    // |   tor+2x+1..........		 	      |
    // |......                     tor+((2*y*x+y/2-1))|
    // |   tor+((2*y*x+y/2))....tor+(x+y+2*x*y-1)     |
    // |______________________________________________| 

    for(int node = 0; node < hosts+tors; node++)
            edge_nodes.push_back(node);


    // Establish connections for all tors
    for(int source = 0; source < 2*y-2; source++)  // y ranges from 0 to 2y-2, (last
	//layer not needed)
     {
       // Even layers (i.e for y=0,2,4.....)
       for(int dest = 0; dest < x; dest++)
	{
	    //
	    int n = x*source + (source/2) + dest;
 	    int n1 = n+x, n2 = n1+1, n3 = n + (2*x+1);
            NodePair p1,p2,p3;
            p1.source = p2.source = p3.source = tor+n;
            p1.destination = tor+n1;
            p2.destination = tor+n2;
            p3.destination = tor+n3;
            pairs.push_back(p1);
            pairs.push_back(p2);
            pairs.push_back(p3);
//	    cout<<p1.source<<"---("<<p1.destination<<","<<p2.destination
//		<<","<<p3.destination<<") connected"<<endl;
 	}	    
	source++;
       // Odd Layers (y=1,3,5...)
       for(int dest = 0; dest < x+1; dest++)
	{
	    //
	    int n = x*source + (source/2) + dest;
 	    int n1 = n+x, n2 = n1+1, n3 = n+1;
            NodePair p1,p2,p3;
            p1.source = p2.source = p3.source = tor+n;
            p1.destination = tor+n1;
            p2.destination = tor+n2;
            p3.destination = tor+n3;
	    if(dest!=0)  {   /* No need to connect for first column */
            	pairs.push_back(p1);
//	        cout<<p1.source<<"---"<<p1.destination<<"connected"<<endl;
	     }
	    if(dest!=x) {  /* No need to connect for last column */
            	pairs.push_back(p2);
                pairs.push_back(p3);
//	        cout<<p2.source<<"---"<<p2.destination<<"connected"<<endl;
//	        cout<<p3.source<<"---"<<p3.destination<<"connected"<<endl;
 	     }
 	}	    
     }

    // Add and establish connections between tors and hosts 
    for(int source = 0; source < tors; source++)
     {
       for(int dest =0; dest < k; dest++)
        {
	  //Cores
          NodePair p;
          p.source = (tor+source);
          p.destination = host + (source*k) + dest;
	  p.isHost = 1;
          pairs.push_back(p);
//	  cout<<p.source<<"---"<<p.destination<<"connected"<<endl;
	}
     }
    }
    result->edge_nodes = edge_nodes;

    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
 //   cout<<"Built"<<endl;
    TRACE("TopologyFactory::createTopologyCell <--");

    return result;
}

