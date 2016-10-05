/******************************************************************************

    source file TopologyFactoryFatTree.cpp with implementation of
	createTopologyFatTree (adapted based on other topologies)
    Author: Suraj  October 5, 2016  Updated number of links and
	addressing format for fat-tree routing
******************************************************************************/
/*
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

Topology* TopologyFactory::createTopologyFatTree(const TString &description)
{
    TRACE("TopologyFactory::createTopologyFatTree -->");

    Topology* result = new Topology();

    int k = atoi(description.at(1).c_str()); // No of pods
    if(k%4!=0) k=k+(4-k%4);     //Move to next whole number divisible by k

    int hosts = k*k*k/4, edges = k*k/2, aggs = k*k/2, cores = k*k/4;
    //cout<<"total="<<hosts+edges+aggs+cores<<endl;
    result->extras.clear();
    result->extras.push_back(k);
    result->number_of_nodes   = hosts + edges + aggs + cores; // Total Nodes in 
                        // all three layers
    result->is_directed       = description.size()<4 ? false :
        (atoi(description.at(3).c_str()) != 0);
    result->number_of_qos     = 0;

    int number_of_links = k*k*k/2 + k*k*k/2 + k*k*k/2;
    result->link_list         = new LinkList(number_of_links);

    if (!(last_description == description))
    {

    int addresses[edges+hosts+aggs+cores][9]; // four layers

    last_description = description;
    pairs.clear();
    edge_nodes.clear();
//    cout<<"k="<<k<<"nodes="<<hosts+edges+aggs+cores<<endl;

    // We label the node from 0 as follows:
    // Host Nodes: 0 to k*k*k/4 -1
     int host=0;
    // Edge Nodes: k*k*k/4 to k*k*k/4 + k*k/2 -1
     int edge=host+hosts;
    // Aggregate Nodes: k*k*k/4 + k*k/2 to k*k*k/4 + k*k -1
     int agg=edge+edges;
    // Core Nodes: k*k*k/4 + k*k to k*k*k/4 + k*k + k -1 
     int core=agg+aggs;

    // cout<<"host="<<host<<"edge="<<edge<<"agg="<<agg<<"core="<<core<<endl;
    cout<<"Fattree("<<k<<"): ";
    cout<<"hosts="<<hosts<<",edges="<<edges<<",aggs="<<aggs<<",cores="<<cores<<endl;
    cout<<"links="<<number_of_links;

    // Add nodes for edges
    for(int node = host; node < hosts; node++) {
        edge_nodes.push_back(node);
        genFatTreeAddress(node,k/2,addresses[node],5);
   //     cout<<"host="<<node<<"Address="<<FormattedFatTreeAdd(addresses[node],5)<<endl;
     }
    for(int node = 0;node <edges;node++)
       genFatTreeAddress(k*k*k*k/16 + node,k/2,addresses[edge+node],5);
    for(int node = 0;node <aggs;node++)
       genFatTreeAddress(2*k*k*k*k/16 + node,k/2,addresses[agg+node],5);
    for(int node = 0;node <cores;node++)
       genFatTreeAddress(3*k*k*k*k/16 + node,k/2,addresses[core+node],5);
      
    //Add to TOR
//    for(int node = edge; node < edges; node++)
//        edge_nodes.push_back(node);
     int conns=0;
    // Add edges and establish connections for edges
    for(int source = 0; source < edges; source++)
     {
       edge_nodes.push_back(edge+source); // all nodes are edge-nodes
//       cout<<"edge="<<edge+source<<"Address="<<FormattedFatTreeAdd(addresses[edge+source],4)<<endl;
       for(int dest = 0; dest < k/2; dest++)
        {
            //Hosts
            NodePair x;
            x.source = (edge+source);
            //x.destination = (source%(k/2))*(k/2)+dest+host;
            x.destination = (source)*(k/2)+dest+host;
            x.isHost = 1;  // if the pair has a host at one end
            pairs.push_back(x);
	    conns++;
//            cout<<x.source<<FormattedFatTreeAdd(addresses[x.source],5)<<"[Edge]---"<<x.destination<<FormattedFatTreeAdd(addresses[x.destination],5)<<"[Host] connected"<<endl;

            //Aggs
            NodePair y;
            y.source = (edge+source);
            y.destination = (source/(k/2))*(k/2)+dest+agg;
            pairs.push_back(y);
	    conns++;
 //           cout<<y.source<<FormattedFatTreeAdd(addresses[y.source],5)<<"[Edge]---"<<y.destination<<FormattedFatTreeAdd(addresses[y.destination],5)<<"[Agg] connected"<<endl;
            //cout<<endl;
        }
     }

    // Add and establish connections for aggs
//    cout<<" aggs "<<aggs;
    for(int source = 0; source < aggs; source++)
     {
       edge_nodes.push_back(agg+source);
 //      genFatTreeAddress(2*k*k*k + source,k,addresses[agg+source],4);
//       cout<<"agg="<<agg+source<<"Address="<<FormattedFatTreeAdd(addresses[agg+source],4)<<endl;
       for(int dest =0; dest < k/2; dest++)
        {
          //Cores

          NodePair x;
          x.source = (agg+source);
          x.destination = (source%(k/2))*(k/2)+dest+core;
          pairs.push_back(x);
	  conns++;
  //        cout<<x.source<<FormattedFatTreeAdd(addresses[x.source],5)<<"[Agg]---"<<x.destination<<FormattedFatTreeAdd(addresses[x.destination],5)<<"[Core] connected"<<endl;
//        cout<<" source "<<source<<" destination "<<dest<<endl;
        }

     }

//    cout<<"loop over";
//     Add nodes for edges
    for(int node = 0; node < cores; node++) {
       edge_nodes.push_back(core+node);
//       genFatTreeAddress(3*k*k*k + node,k,addresses[core+node],4);
//       cout<<"core="<<core+node<<"Address="<<FormattedFatTreeAdd(addresses[core+node],4)<<endl;
     }
    cout<<",Conns="<<conns*2<<endl;
    }
    result->edge_nodes = edge_nodes;
    result->first_node = 0;
    result->last_node = hosts-1;

    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
//    exportJSON(nodepairs, result);
    TRACE("TopologyFactory::createTopologyFatTree <--");

    return result;
}
