Topology* TopologyFactory::createTopologyFatTree(const TString &description)
{
    TRACE("TopologyFactory::createTopologyFatTree -->");

    Topology* result = new Topology();

    int k = atoi(description.at(1).c_str()); // No of pods
    if(k%4!=0) k=k+(4-k%4);     //Move to next whole number divisible by k

    int hosts = k*k*k/4, edges = k*k/2, aggs = k*k/2, cores = k*k/4;
    //cout<<"hosts="<<hosts+edges+aggs+cores<<endl;
    result->number_of_nodes   = hosts + edges + aggs + cores; // Total Nodes in 
                        // all three layers
    result->is_directed       = description.size()<4 ? false :
        (atoi(description.at(3).c_str()) != 0);
    result->number_of_qos     = 0;
    int number_of_links = k*k*k/2 + k*k*k/2 + 2*k;
    result->link_list         = new LinkList(number_of_links);

    if (!(last_description == description))
    {

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

     result->first_node = edge;
     result->last_node = agg -1;
    // cout<<"host="<<host<<"edge="<<edge<<"agg="<<agg<<"core="<<core<<endl;
    // cout<<"hosts="<<hosts<<"edges="<<edges<<"aggs="<<aggs<<"cores="<<cores<<endl;

    // Add nodes for edges
    for(int node = host; node < hosts; node++)
        edge_nodes.push_back(node);

    //Add to TOR
   // for(int node = edge; node < edges; node++)
       // edge_nodes.push_back(node);

    // Add edges and establish connections for edges
    for(int source = 0; source < edges; source++)
     {
       edge_nodes.push_back(edge+source); // all nodes are edge-nodes
       for(int dest = 0; dest < k/2; dest++)
        {
            //Hosts
            NodePair x;
            x.source = (edge+source);
            x.destination = (source%(k/2))*(k/2)+dest+core;
            x.isHost = 1;  // if the pait has a host at one end
            pairs.push_back(x);
           // cout<<x.source<<"---"<<x.destination<<"connected"<<endl;

            //Aggs
            NodePair y;
            y.source = (edge+source);
            y.destination = (source/(k/2))*(k/2)+dest+agg;
            pairs.push_back(y);
            //cout<<y.source<<"---"<<y.destination<<"connected"<<endl;
            //cout<<endl;
        }
     }

    // Add and establish connections for aggs
    cout<<" aggs "<<aggs;
    for(int source = 0; source < aggs; source++)
     {
       edge_nodes.push_back(agg+source);
       for(int dest =0; dest < k/2; dest++)
        {
          //Cores

          NodePair x;
          x.source = (agg+source);
          x.destination = (source%(k/2))*(k/2)+dest+core;
          pairs.push_back(x);
//        cout<<x.source<<"---"<<x.destination<<"connected"<<endl;
//        cout<<" source "<<source<<" destination "<<dest<<endl;
        }

     }

//    cout<<"loop over";
//     Add nodes for edges
    for(int node = 0; node < cores; node++)
        edge_nodes.push_back(core+node);
    }
result->edge_nodes = edge_nodes;

   // result->tor_nodes = tor_nodes;

    // Now build nodepairs list.
    NodePairDeque nodepairs(pairs);
    build(nodepairs, result);
//    exportJSON(nodepairs, result);
    TRACE("TopologyFactory::createTopologyFatTree <--");

    return result;
}


