/******************************************************************************

    source file TopologyFactorySlimNet.cpp with implementation of
	createTopologySlimNet (adapted based on other topologies)
    Author: Suraj  May2016
******************************************************************************/


//------------------------------------------------------------------------------
// Create a SlimNet graph with k levels of nodes
//
// createTopologySlimNet()
//------------------------------------------------------------------------------
Topology* TopologyFactory::createTopologySlimNet(const TString &description)
{
    TRACE("TopologyFactory::createTopologySlimNet -->");

    Topology* result = new Topology();

    int k = atoi(description.at(1).c_str()); // No of levels (recursive depth)
    int hosts_per_rack = 2, racks_per_level = 8; 

    if(description.size() > 2) 
	hosts_per_rack = atoi(description.at(2).c_str());
	
    if(description.size() > 3) 
	racks_per_level = atoi(description.at(3).c_str());

    int aggs = pow (racks_per_level, k+1);
    int hosts = hosts_per_rack*aggs;

    cout<<"hosts="<<hosts+aggs<<endl;
    result->number_of_nodes   = hosts + aggs; // Total Nodes in 
			// all three layers
    result->is_directed       = description.size()<5 ? false :
        (atoi(description.at(4).c_str()) != 0);
    result->number_of_qos     = 0;
    result->link_list         = new LinkList(result->number_of_nodes);

    if (!(last_description == description))
    {

    last_description = description;
    // Make list of all posible nodepairs and remove link with
    pairs.clear();
    edge_nodes.clear();
    cout<<"k="<<k<<"total nodes="<<hosts+aggs<<endl;

    // We label the node from 0 as follows:
    // Host Nodes: 0 to pow(racks_per_level, k+1)*hosts_per_rack
     int host=0;
    // Aggregate Nodes: pow(racks_per_level, k+1)*hosts_per_rack to 
	//	pow(racks_per_level, k+1)*(hosts_per_rack+1)
     int agg=host+hosts;

     int addresses[hosts][k+1]; 
     for(int node= host;node < hosts; node++) {
	  genSlimNetAddress(node,racks_per_level,addresses[node],k+1);
//	  cout<<"node="<<node<<"Address="<<FormattedSlimNetAdd(addresses[node],k+1)<<endl;
	}

     cout<<"host="<<host<<"agg="<<agg<<endl;

    // Add nodes for hosts
    for(int node = host; node < hosts; node++)
	edge_nodes.push_back(node);

    // Add aggs and establish connections
    for(int source = 0; source < aggs; source++)
     {
       edge_nodes.push_back(agg+source); // all nodes are edge-nodes
       for(int dest = 0; dest < hosts_per_rack; dest++)
	{
	    //Links to Hosts
            NodePair x;
            x.source = (agg+source);
            x.destination = source*(hosts_per_rack)+dest+host;
	    x.isHost = 1;  // if the pair has a host at one end
            pairs.push_back(x);
	    cout<<x.source<<"["<<FormattedSlimNetAdd(addresses[source],k+1)<<"]---"<<x.destination<<" connected"<<endl;
	} 
	
	    // Links within the level i<-->i+1
	    int j = source+racks_per_level-(source%racks_per_level);
            NodePair y;
            y.source = (agg+source);
            y.destination = agg+(source+1)%j;
            pairs.push_back(y);
	    cout<<y.source<<"["<<FormattedSlimNetAdd(addresses[source],k+1)<<"]---"<<y.destination<<"["<<FormattedSlimNetAdd(addresses[source+1],k+1)<<"] connected"<<endl;

	    // Links in same level i<-->i+3
            NodePair z;
            z.source = (agg+source);
            z.destination = agg+(source+3)%j;
            pairs.push_back(z);
	    cout<<z.source<<"["<<FormattedSlimNetAdd(addresses[source],k+1)<<"]---"<<z.destination<<"["<<FormattedSlimNetAdd(addresses[source+3],k+1)<<"] connected"<<endl;

	    // Links outside the base level
	    for(;j<aggs;j++) {
//		cout<<"Evaluating source & j"<<source<<" "<<j<<endl;
		int* addI=addresses[source];
		int lsbI=addI[k];
		int* addJ=addresses[j];
		int lsbJ=addJ[k];
//		cout<<"Add(source)=("<<source<<")="<<FormattedSlimNetAdd(addI,k+1)<<",Add(j)=("<<j<<")="<<FormattedSlimNetAdd(addJ,k+1)<<""<<endl;
		if(lsbI!=lsbJ) continue;

  	     //Recursive  - High Complexity - Unfortunate
		int t=0;
		while(t<k+1) {
		  int msbI=addI[t];
		  int msbJ=addJ[t++];

	     		//As per definition
       		   int fxy = (lsbI==0) ? ((racks_per_level-1)-msbI) : ((msbI|lsbI)&((racks_per_level-1)-(msbI&lsbI)));
		   //cout<<lsbI<<" "<<msbI<<" "<<fxy<<endl;
  		   // Connect if either of this is true 
  		   if(fxy==msbJ) {
//			cout<<"total_bridges="<<total_bridges<<" i="<<i<<" m="<<m<<" ae="<<i*total_bridges+m;
            		NodePair p;
            		p.source = (agg+source);
            		p.destination = (agg+j);
            		pairs.push_back(p);
//			cout<<"Add(source)="<<FormattedSlimNetAdd(addI,k+1)<<",Add(j)="<<FormattedSlimNetAdd(addJ,k+1)<<endl;
	    		cout<<p.source<<"["<<FormattedSlimNetAdd(addresses[source],k+1)<<"]---"<<p.destination<<"["<<FormattedSlimNetAdd(addresses[j],k+1)<<"] connected"<<endl;
		}			
	     } 	
	    }
     }

    }

    result->edge_nodes = edge_nodes;
    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
    TRACE("TopologyFactory::createTopologySlimNet <--");

    return result;
}

// Generate Address in reverse order - used for efficiency in Hyscale
// Decimal to X base
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

// Print SlimNet Address 
   string FormattedSlimNetAdd(int *addr,int level) {
     char address[10]; // 10 levels
     std::sprintf(address,"<%d",addr[0]); 
     for(int i=1;i<level;i++)
	std::sprintf(address,"%s,%d",address,addr[i]);
     std::sprintf(address,"%s>",address);
     return string(address);
}
