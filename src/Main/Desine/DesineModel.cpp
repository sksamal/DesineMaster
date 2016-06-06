/******************************************************************************

    source file DesineModel.cpp for class: DesineModel

    See header file for details

    Author : T. Kleiberg
    Version: 1
    Date   : June 2004
    Feb8,2016 : Merged changes
******************************************************************************/

/******************************************************************************
*      ## no path (blocked) ## eccepted ## Rejected Setup ## Rejected QoS     *
*#############################################################################*
* tp   ## total   blocked   ## tot acc  ## tot rej_setup  ## tot rej_qos      *
* hops ##                   ## total    ## total          ## total            *
* flow ## total   blocked   ## tot acc  ## tot rej_setup  ## tot rej_qos      *
*******************************************************************************/



// Include(s)
#include "Algorithms/Algorithm.h"
#include "LinkStateUpdate/LinkStateUpdateVisitor.h"
#include "Main/Data.h"
#include "Main/Desine/DesineData.h"
#include "Main/Desine/DesineModel.h"
#include "Network/Flow.h"
#include "Network/FlowManager.h"
#include "Network/BufferList.cpp"
#include "Network/BufferData.cpp"
#include "Network/Path.h"
#include "Network/Topology.h"
#include "Network/TopologyFactory.h"
#include "Parameter/Parameters.h"
#include "Utils/Time.h"
#include "Utils/Types.h"
#include "Utils/Util.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
using Types::uli;
using Types::ldouble;
using std::ostringstream;
using std::vector;


// Constanst(s)

// Variable(s)

// Function(s) definitions


//------------------------------------------------------------------------------
//  constructor DesineModel::DesineModel(char* input)
//------------------------------------------------------------------------------
DesineModel::DesineModel(Parameters* parameters,
    Algorithm* algorithm, LinkStateUpdateVisitor* lsuv) : Model()
{
    TRACE("DesineModel::DesineModel -->");
    this->parameters = parameters;
    this->algorithm = algorithm;
    this->lsuv = lsuv;

    ostringstream description;
    algorithm->print(description);
    if (lsuv)
    {
        lsuv->print(description);
    }
    name = description.str();
    TRACE("DesineModel::DesineModel <--");
}


//------------------------------------------------------------------------------
//  destructor DesineModel::~DesineModel()
//------------------------------------------------------------------------------
DesineModel::~DesineModel()
{
    TRACE("DesineModel::~DesineModel -->");
    // Empty
    TRACE("DesineModel::~DesineModel <--");
}


//------------------------------------------------------------------------------
//  void execute();
//------------------------------------------------------------------------------
Data* DesineModel::execute(const int &iterations)
{
    TRACE("DesineModel::execute -->");

    // Initialization
    int number_of_qos = parameters->qos_cons.size();
    long flows = parameters->flows;

    Time* time1 = new Time();
    DesineData* result = new DesineData(name, number_of_qos, flows);
    TopologyFactory top_fct(parameters->l_capacity,parameters->hl_capacity);
	
	cout<<"Sim Type "<<parameters->Sim_Type;
	cout<<" Buf Type "<<parameters->Buff_Type;
    // Start iteration



    for (int iteration = 0; iteration < iterations; ++iteration)
    {
        TRACE("DesineModel::execute: starting iteration: " << iteration);


		// Build topology
	    Topology *topology = (Topology *) 0;
	    do
	    {
	    	if (topology) delete topology;
	    	topology = (Topology *) 0;
	    	topology = top_fct.create(parameters->topology);
	    } while(!topology->isConnected());

	    // Topology accepts the lsu visitor
	    if (lsuv)
	    {
	    	lsuv->release();
	        lsuv->setTime(time1);
	        topology->accept(*lsuv);
	    }
        DesineData data(name, number_of_qos, flows);
        FlowManager flow_man(parameters, topology);
        BufferList bl;

        time1->reset();
        flow_man.setTime(time1);


        uli update_num = 0;
        uli flow_rejected_setup = 0;
        uli flow_total = 0;
        uli flow_accepted = 0;
        uli flow_blocked = 0;
        uli hop_total = 0;
        vector<uli> flow_rejected_qos(number_of_qos);

        ldouble tp_total = 0.0;
        ldouble tp_accepted = 0.0;
        ldouble tp_blocked = 0.0;
        ldouble tp_rejected_setup = 0.0;
       // ldouble tp_rejected_qos = 0.0;
        //added for obs
        ldouble tp_process = 0.0;

        //----------------------------------------------------------------------
        //
        //   S T A R T
        //
        //----------------------------------------------------------------------
        for (long flowcounter = 0; flowcounter < flows; ++flowcounter)
        {

            bool steady(flowcounter > parameters->warmup);
//		cout<<" flow# "<<flowcounter;
//	cout<<" data blocked "<<tp_blocked;
//	cout<<" data total "<<tp_total;
//	cout<<" flow blocked# "<<flow_blocked;
            // update flow list to see whicd expired

       //     milliseconds ms = duration_cast< milliseconds >(
       //         system_clock::now().time_since_epoch());

                        flow_man.update();

                        int source;
						int dest;

            int input_type = topology->getInputType();


            	if (input_type == 0)
			{
            		 dest = topology->getRandomTorNode(parameters->endpoints);
            		source = topology->getRandomTorNode(parameters->endpoints);
            		//source =topology->getTor(host);
            		//int dst = host;

                	while (source==dest)
                	{
                    	dest = topology->getRandomTorNode(parameters->endpoints);
                    	//dest=topology->getTor(host);
                	}
                }
            	else
            	{
            		int num_pair_sd = topology->getNumSDpairs();

            		int index = ( (int) ceil( num_pair_sd *
            		                              parameters->endpoints->generate() ) );

            				 source = topology->getflowsrc(index - 1);
            				 dest = topology->getflowdest(index - 1);
            	}

            	    	//added for obs by suraj y.
            	double tstart = flow_man.getGLobalTime();
            	tstart  += parameters->f_arrival->generate();
                flow_man.setGLobalTime(tstart);
            	double tend;



            	    	if(parameters->f_random->generate()>80)
            	    	{	
				//cout<<" long flow";

				tend = tstart + parameters->f_duration1->generate();

            // Create flow
            Flow flow(flow_man.create(source, dest, tstart, tend));


#ifndef NO_THROUGHPUT
            //PRINTLN("\t tp:" << flow_man.getThroughput() << " fs:" << flow_man.size());
            //Update throughput
            data.throughput[flowcounter].add(flow_man.getThroughput());
            data.numflows[flowcounter].add(flow_man.size());
#endif //NO_THROUGHPUT

            // Find path for flow
            Path path(algorithm->compute(flow));
            if (path.size() > 0)   // flow is routed succesfully)
            {
                // Setup the path
                flow.setPath(path);
                int result_code = flow.allocate();

                //PRINT("\t rc:" << result_code << " ps:" <<path.size());
                switch(result_code)
                {
                    case 0: // flow accepted
                        flow_man.accept(flow);
                        if (steady)
                        {
                            flow_accepted++;
                            tp_accepted += flow.getRequestedCapacity()*flow.getDuration()/100;
                            tp_process +=path.size()*0.001;
                        }
                        break;
                    case 1: // flow rejected in setup
                        if (steady)
                        {
                            flow_rejected_setup++;
                            tp_rejected_setup += flow.getRequestedCapacity()*flow.getDuration()/100;
                        }
                        break;
             /*       default: // flow rejected QoS
                        if (steady)
                        {
                            flow_rejected_qos[result_code-10]++;
                            tp_rejected_qos += flow.getRequestedCapacity();
                       }
                        break;*/
                } // end: switch
            } // end: if (path
            else // path length = 0
            {
                if (steady)
                { //  cout << " place 1 ";
                     flow_blocked++;
//			cout<<" blocked cap: "<<flow.getRequestedCapacity();
                     tp_blocked += flow.getRequestedCapacity()*flow.getDuration()/100;
                }
            } // end: path length

            // Get data
            if (steady)
            {
                tp_total += flow.getRequestedCapacity()*flow.getDuration()/100;
                data.time_comp.add(flow.getTime());
                flow_total++;
                hop_total += path.size();
            }
            	    	}
            	    	else
            	    	{
            	    		tend   = tstart + parameters->f_duration2->generate();

            	    		if(parameters->Sim_Type==1)
            	    		{
            	    			//cout<<" OCS Only ";
            	    			Flow flow(flow_man.create(source, dest, tstart, tend));


#ifndef NO_THROUGHPUT
            //PRINTLN("\t tp:" << flow_man.getThroughput() << " fs:" << flow_man.size());
            //Update throughput
            data.throughput[flowcounter].add(flow_man.getThroughput());
            data.numflows[flowcounter].add(flow_man.size());
#endif //NO_THROUGHPUT

            // Find path for flow
            Path path(algorithm->compute(flow));
            if (path.size() > 0)   // flow is routed succesfully)
            {
                // Setup the path
                flow.setPath(path);
                int result_code = flow.allocate();

                //PRINT("\t rc:" << result_code << " ps:" <<path.size());
                switch(result_code)
                {
                    case 0: // flow accepted
                        flow_man.accept(flow);
                        if (steady)
                        {
                            flow_accepted++;
                            tp_accepted += flow.getRequestedCapacity()*flow.getDuration()/100;
                            tp_process +=path.size()*0.001;
                        }
                        break;
                    case 1: // flow rejected in setup
                        if (steady)
                        {
                            flow_rejected_setup++;
                            tp_rejected_setup += flow.getRequestedCapacity()*flow.getDuration()/100;
                        }
                        break;
                /*    default: // flow rejected QoS
                        if (steady)
                        {
                            flow_rejected_qos[result_code-10]++;
                            tp_rejected_qos += flow.getRequestedCapacity();
                       }
                        break;*/
                } // end: switch
            } // end: if (path
            else // path length = 0
            {
                if (steady)
                { 	
//			cout<<" place 2 ";
                     flow_blocked++;
                     tp_blocked += flow.getRequestedCapacity()*flow.getDuration()/100;
                }
            } // end: path length

            // Get data
            if (steady)
            {
                tp_total += flow.getRequestedCapacity()*flow.getDuration()/100;
                data.time_comp.add(flow.getTime());
                flow_total++;
                hop_total += path.size();
            }
            	    		}
            	    		else{

				if(steady){
            	    		int id= (source)*topology->getNumEdgeNodes()+dest;

            	    		bool present = bl.exists(id);

            	    		if(!present)
            	    		{
            	    			double tstart1=tstart+100;
            	    			double dur= tend-tstart;

            	    			BufferData bf;
            	    			bf.create(id, source, dest, tstart1, tend, dur);
            	    			bl.accept(bf);
            	    		}
            	    		else
            	    		{
            	    			bool buff_full = bl.addCap(id,(tend-tstart));


            	    			if(buff_full & parameters->Buff_Type !=2 )
            	    			{
            	    				//cout<<" Buffer Triggered";

            	    				BufferData bf1 = bl.getBuffer(id);
            	    				Flow flow(flow_man.create(bf1.getSource(),bf1.getDestination(),bf1.getTstart(),bf1.getTend()));
            	    				bl.erase(bf1.getTstart());

            	    				#ifndef NO_THROUGHPUT
            	    				            //PRINTLN("\t tp:" << flow_man.getThroughput() << " fs:" << flow_man.size());
            	    				            //Update throughput
            	    				            data.throughput[flowcounter].add(flow_man.getThroughput());
            	    				            data.numflows[flowcounter].add(flow_man.size());
            	    				#endif //NO_THROUGHPUT

            	    				            // Find path for flow
            	    				            Path path(algorithm->compute(flow));
            	    				            if (path.size() > 0)   // flow is routed succesfully)
            	    				            {
            	    				                // Setup the path
            	    				                flow.setPath(path);
            	    				                int result_code = flow.allocate();

            	    				                //PRINT("\t rc:" << result_code << " ps:" <<path.size());
            	    				                switch(result_code)
            	    				                {
            	    				                    case 0: // flow accepted
            	    				                        flow_man.accept(flow);
            	    				                        if (steady)
            	    				                        {
            	    				                            flow_accepted+=bf1.getFlows();
            	    				                            tp_accepted += flow.getRequestedCapacity()*flow.getDuration()/100;
            	    				                            tp_process +=path.size()*.02;
            	    				                        }
            	    				                        break;
            	    				                    case 1: // flow rejected in setup
            	    				                        if (steady)
            	    				                        {
            	    				                            flow_rejected_setup+=bf1.getFlows();
            	    				                            tp_rejected_setup += flow.getRequestedCapacity()*flow.getDuration()/100;
            	    				                        }
            	    				                        break;
            	    				                /*    default: // flow rejected QoS
            	    				                        if (steady)
            	    				                        {
            	    				                            flow_rejected_qos[result_code-10]+=bf1.getFlows();
            	    				                            tp_rejected_qos += flow.getRequestedCapacity();
            	    				                       }
            	    				                        break;*/
            	    				                } // end: switch
            	    				            } // end: if (path
            	    				            else // path length = 0
            	    				            {
            	    				                if (steady)
            	    				                {//	cout<<"place 3 ";
            	    				                     flow_blocked+=bf1.getFlows();
            	    				                     tp_blocked += flow.getRequestedCapacity()*flow.getDuration()/100;
            	    				                }
            	    				            } // end: path length

            	    				            // Get data
            	    				            if (steady)
            	    				            {
            	    				                tp_total += flow.getRequestedCapacity()*flow.getDuration()/100;
            	    				                data.time_comp.add(flow.getTime());
            	    				                flow_total+=bf1.getFlows();
            	    				                hop_total += path.size();
            	    				            }
            	             	    			}

            	    		}

			   }
            	    	}
				
            	    	}
            	    	while(bl.update(flow_man.getGLobalTime()) & parameters->Buff_Type!=1 & parameters->Sim_Type !=1 & steady)
            	    	{
            	    		BufferData bf2=bl.getFirstBuf();
            	    		Flow flow(flow_man.create(bf2.getSource(), bf2.getDestination(), bf2.getTstart(), bf2.getTend()));


    	    				bl.erase(bf2.getTstart());

    	    				#ifndef NO_THROUGHPUT
    	    				            //PRINTLN("\t tp:" << flow_man.getThroughput() << " fs:" << flow_man.size());
    	    				            //Update throughput
    	    				            data.throughput[flowcounter].add(flow_man.getThroughput());
    	    				            data.numflows[flowcounter].add(flow_man.size());
    	    				#endif //NO_THROUGHPUT

    	    				            // Find path for flow
    	    				            Path path(algorithm->compute(flow));
    	    				            if (path.size() > 0)   // flow is routed succesfully)
    	    				            {
    	    				                // Setup the path
    	    				                flow.setPath(path);
    	    				                int result_code = flow.allocate();

    	    				                //PRINT("\t rc:" << result_code << " ps:" <<path.size());
    	    				                switch(result_code)
    	    				                {
    	    				                    case 0: // flow accepted
    	    				                        flow_man.accept(flow);
    	    				                        if (steady)
    	    				                        {
    	    				                            flow_accepted+=bf2.getFlows();
    	    				                            tp_accepted += flow.getRequestedCapacity()*flow.getDuration()/100;
    	    				                            tp_process +=path.size()*.02;
    	    				                        }
    	    				                        break;
    	    				                    case 1: // flow rejected in setup
    	    				                        if (steady)
    	    				                        {
    	    				                            flow_rejected_setup+=bf2.getFlows();
    	    				                            tp_rejected_setup += flow.getRequestedCapacity()*flow.getDuration()/100;
    	    				                        }
    	    				                        break;
    	    				                    /*default: // flow rejected QoS
    	    				                        if (steady)
    	    				                        {
    	    				                            flow_rejected_qos[result_code-10]+=bf2.getFlows();
    	    				                            tp_rejected_qos += flow.getRequestedCapacity();
    	    				                       }
    	    				                        break;*/
    	    				                } // end: switch
    	    				            } // end: if (path
    	    				            else // path length = 0
    	    				            {
    	    				                if (steady)
    	    				                {  // cout<<" place 4 ";
    	    				                     flow_blocked+=bf2.getFlows();
    	    				                     tp_blocked += flow.getRequestedCapacity()*flow.getDuration()/100;
    	    				                }
    	    				            } // end: path length

    	    				            // Get data
    	    				            if (steady)
    	    				            {
    	    				                tp_total += flow.getRequestedCapacity()*flow.getDuration()/100;
    	    				                data.time_comp.add(flow.getTime());
    	    				                flow_total+=bf2.getFlows();
    	    				                hop_total += path.size();
    	    				            }
            	    	}


        } // end: for (flowcounter...all flows have been generated
        //----------------------------------------------------------------------
        //
        //   E N D
        //
        //----------------------------------------------------------------------
        if(lsuv)
        {
            update_num = lsuv->getUpdateNum();
            data.update_num.add(update_num);
            //lsuv->release();
        }

//	cout<<" tp total "<<tp_total;
//	cout<<" flow total "<<flow_total;
//	cout<<" tp accept"<<tp_accepted;
//	cout<<" flow accept"<<flow_accepted;
//	cout<<" tp blocked"<<tp_blocked;
//	cout<<" flow blocked "<<flow_blocked;
//	cout<<" tp rejected "<<tp_rejected_setup;
//	cout<<" flow rejected "<<flow_rejected_setup;	
        data.flow_total.add(flow_total);
        data.flow_accepted.add(flow_accepted);
        data.flow_blocked.add(flow_blocked);
        data.flow_rejected_setup.add(flow_rejected_setup);
        data.hop_total.add(hop_total);
        data.tp_total.add(tp_total);
        data.tp_accepted.add(tp_accepted);
        data.tp_blocked.add(tp_blocked);
        data.tp_rejected_setup.add(tp_rejected_setup);
        //data.tp_rejected_qos.add(tp_rejected_qos);
        data.tp_process.add(tp_process); //added for obs by suraj y.

        /*for (uli ix = 0; ix < flow_rejected_qos.size(); ++ix)
        {
            data.flow_rejected_qos[ix].add(flow_rejected_qos[ix]);
        }*/

        (*result) += data;
		delete topology;
    } // end: for (int ai

    delete time1;
    TRACE("DesineModel::execute <--");
    return result;
}
