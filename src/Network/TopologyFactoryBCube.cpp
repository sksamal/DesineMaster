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

//    long hconns =0, lconns = 0 , oconns=0;
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
     for(int node= 0;node < tors; node++) {
	  genBCubeAddress(node,n,addresses[node],k+1);
	  cout<<"node="<<node<<"Address="<<FormattedBCubeAdd(addresses[node],k+1)<<endl;
	}

 //    cout<<"host="<<host<<"tor="<<tor<<endl;

    // Add nodes for hosts
    for(int node = host; node < hosts; node++)
	edge_nodes.push_back(node);

    // Add tors and establish connections
    for(int level = 0; level < k+1; level++) 
    {
    int start = pow(n,level);
    int leveltors = pow(n,level+1) - pow(n,level);
    for(int source = 0; source < leveltors; source++)
     {
        
       edge_nodes.push_back(tor+start+source); // all nodes are edge-nodes
       for(int dest = 0; dest < n; dest++)
	{
	    //Links to Hosts
            NodePair x;
            x.source = (tor+start+source);
            x.destination = source*(n)+dest+host;
	    x.isHost = 1;  // if the pair has a host at one end
            pairs.push_back(x);
//	    hconns++;
	    cout<<x.source<<"["<<FormattedBCubeAdd(addresses[source+start],k+1)<<"]---"<<x.destination<<"[Host] connected"<<endl;
	} 
	
//	    lconns++;
//	    cout<<z.source<<"["<<FormattedBCubeAdd(addresses[source],k+1)<<"]---"<<z.destination<<"["<<FormattedBCubeAdd(addresses[j+(source+3)%racks_per_level],k+1)<<"] connected"<<endl;

	    // Links outside the base level
//	    for(int j=0;j<tors;j++) {
//		cout<<"Evaluating source & j"<<source<<" "<<j<<endl;
//		int* addI=addresses[source];
//		int lsbI=addI[k];
//		int* addJ=addresses[j];
//		int lsbJ=addJ[k];
//		cout<<"Add(source)=("<<source<<")="<<FormattedBCubeAdd(addI,k+1)<<",Add(j)=("<<j<<")="<<FormattedBCubeAdd(addJ,k+1)<<""<<endl;
//		if(lsbI!=lsbJ) continue;

  	     //Recursive  - High Complexity - Unfortunate
//		int t=0;
//		while(t<k+1) {
//		  int msbI=addI[t];
//		  int msbJ=addJ[t++];

	     		//As per definition
//       		   int fxy = (lsbI==0) ? ((n-1)-msbI) : ((msbI|lsbI)&((n-1)-(msbI&lsbI)));
		   //cout<<lsbI<<" "<<msbI<<" "<<fxy<<endl;
  		   // Connect if either of this is true 
 // 		   if(fxy==msbJ) {
//			cout<<"total_bridges="<<total_bridges<<" i="<<i<<" m="<<m<<" ae="<<i*total_bridges+m;
   //         		NodePair p;
   //         		p.source = (tor+source);
   //         		p.destination = (tor+j);
   //        		pairs.push_back(p);
//			cout<<"Add(source)="<<FormattedBCubeAdd(addI,k+1)<<",Add(j)="<<FormattedBCubeAdd(addJ,k+1)<<endl;
//			oconns++;
//	    		cout<<p.source<<"["<<FormattedBCubeAdd(addresses[source],k+1)<<"]---"<<p.destination<<"["<<FormattedBCubeAdd(addresses[j],k+1)<<"] connected"<<endl;
//		}			
//	     } 	
//	    }
     }
    }

//    cout<<"Conns created: hconns="<<hconns<<" lconns="<<lconns<<" oconns="<<oconns;
//    cout<<" Total conns="<<hconns+lconns+oconns<<" <-bidirectional->="<<2*(hconns+lconns+oconns)<<endl;
//    cout<<" Pairs="<<pairs.size()<<endl;
    }

    result->first_node = hosts;
    result->last_node = hosts+tors-1;
    result->edge_nodes = edge_nodes;
    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
    TRACE("TopologyFactory::createTopologyBCube <--");

    return result;
}
