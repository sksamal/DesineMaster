/******************************************************************************

    source file TopologyFactoryBCube.cpp with implementation of
	createTopologyBCube (adapted based on other topologies)
    Author: Suraj  Sep 2016
******************************************************************************/


//------------------------------------------------------------------------------
// Create a BCube graph with k levels of nodes
//
// createTopologyBCube()
//------------------------------------------------------------------------------
Topology* TopologyFactory::createTopologyBCube(const TString &description)
{
    TRACE("TopologyFactory::createTopologyBCube -->");

    Topology* result = new Topology();

    int k = atoi(description.at(1).c_str()); // No of levels (recursive depth)
    int n = 2;  // Hosts per rack (or hosts in BCube0)

    if(description.size() > 2) // n servers connecting to a rack
	n = atoi(description.at(2).c_str());
	
    int tors = (k+1) * pow (n, k);
    int hosts = pow (n, k+1);

//    cout<<"hosts="<<hosts+tors<<endl;
    result->extras.clear();		
    result->extras.push_back(k);		
    result->extras.push_back(n);		
    result->number_of_nodes   = hosts + tors; // Total Nodes in 
			// all three layers
    result->is_directed       = description.size()<4 ? false :
        (atoi(description.at(3).c_str()) != 0);
    result->number_of_qos     = 0;
    int links = tors*n;
    result->link_list         = new LinkList(links);

    if (!(last_description == description))
    {

    long conns =0;
    last_description = description;
    // Make list of all posible nodepairs and remove link with
    pairs.clear();
    edge_nodes.clear();
    cout<<"k="<<k<<" hosts="<<hosts<<" tors="<<tors<<" total="<<hosts+tors<<endl;
    cout<<"Links="<<links<<" Configuration= ("<<k<<","<<n<<")"<<endl;

    // We label the node from 0 as follows:
    // Host Nodes: 0 to pow(n, k+1) - 1
     int host=0;
    // Aggregate Nodes: pow(n, k+1) to 
	//	(n + k+1) * pow (n, k) - 1 
     int tor=host+hosts;

     int addresses[tors][k+1]; 
     int hostaddr[hosts][k+1]; 
     for(int node= 0;node < tors; node++) {
	 genBCubeAddress(node,n,addresses[node],k+1);
//	  cout<<""<<node<<"["<<FormattedBCubeAdd(addresses[node],k+1)<<"] ";
//        if(node%(tors/(k+1))==(tors/(k+1))-1) 
//          cout<<endl<<endl;
     }

     for(int node= 0;node < hosts; node++) 
	  genBCubeAddress(node,n,hostaddr[node],k+1);
 //    cout<<"host="<<host<<"tor="<<tor<<endl;

    // Add nodes for hosts
    for(int node = host; node < hosts; node++)
	edge_nodes.push_back(node);

    // Add tors and establish connections
    for(int source = 0; source < tors; source++) {
	edge_nodes.push_back(source);
	int level = addresses[source][0];
	int tempnode = 0;
//        cout<<"addr[k+1="<<k+1<<"] "<<addresses[source][k+1]<<endl;
	for(int l=0;l<level;l++) {
	  tempnode+=(addresses[source][k-l]*pow(n,l));
//          cout<<"addr[k-"<<l<<"-"<<k-l<<"] "<<addresses[source][k-l]<<endl;
	}

 	for(int l=level+1;l<k+1;l++) 
	  tempnode+=(addresses[source][k-l+1]*pow(n,l));
 //       cout<<"addr[0] "<<addresses[source][0]<<endl;
	for(int dest = 0; dest < n; dest++)
         {
	  NodePair x;
	  x.source = (tor + source);
	  x.destination = (tempnode + dest*pow(n,level));
	  x.isHost = 1;  // if the pair has a host at one end
          pairs.push_back(x);
	  conns++;
//	  cout<<x.source<<"["<<FormattedBCubeAdd(addresses[source],k+1)<<"]---"<<x.destination<<"[Host]["<<FormattedBCubeAdd(hostaddr[x.destination],k+1)<<"] connected"<<endl;
	 }
    }	
    cout<<"Conns created: conns="<<conns;
    cout<<" Pairs="<<pairs.size()<<endl;
    }

    result->first_node = 0;
    result->last_node = hosts-1;
    result->edge_nodes = edge_nodes;
    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
    TRACE("TopologyFactory::createTopologyBCube <--");

    return result;
}
