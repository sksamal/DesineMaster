/******************************************************************************

    source file FlowManager.cpp for class: FlowManager

    See header file for details

    Author : T. Kleiberg
    Version: 1
    Date   : June 2004
    Feb8,2016 : Suraj merged changes
******************************************************************************/



// Include(s)
#include "Network/Flow.h"
#include "Network/FlowList.h"
#include "Network/FlowManager.h"
#include "Network/Topology.h"
#include "Parameter/Parameters.h"
#include "RandomVariables/ExponentialVar.h"
#include "RandomVariables/RandomVar.h"
#include "RandomVariables/UniformVar.h"
#include "Utils/TraceManager.h"
#include <iostream>
#include "Utils/Types.h"
#include <cmath>
#include <fstream>
using Types::DblVector;


// Constanst(s)

// Variable(s)

// Function(s) definitions

//------------------------------------------------------------------------------
//  FlowManager::FlowManager(Parameters* parameters, Topology* topology)
//------------------------------------------------------------------------------
FlowManager::FlowManager(Parameters* parameters, Topology* topology)
    : parameters(parameters), topology(topology)
{
    TRACE("FlowManager::FlowManager -->");

    flow_id = 0;
    global_time = 0;

    TRACE("FlowManager::FlowManager <--");
}


//------------------------------------------------------------------------------
//  destructor FlowManager::~FlowManager()
//------------------------------------------------------------------------------
FlowManager::~FlowManager()
{
    TRACE("FlowManager::~FlowManager -->");
    // Empty!
    TRACE("FlowManager::~FlowManager <--");
}


//------------------------------------------------------------------------------
//  Flow* FlowManager::create()
//------------------------------------------------------------------------------
Flow FlowManager::create(int source, int dest, double tstart, double tend)
{
    TRACE("FlowManager::create -->");



    	// QoS Constraints
    	int number_of_qos = parameters->qos_cons.size();
   		DblVector qoscons;
    	for (int counter = 0; counter < number_of_qos; ++counter)
    	{
       		qoscons.push_back(parameters->qos_cons[counter]->generate());
   		}
    	double reqc = parameters->f_bandwidth->generate();
               
    	Flow result(flow_id++, source, dest, reqc, qoscons, tstart, tend);
    	result.topology = topology;
    	TRACE("FlowManager::create <--");
   		return result;
    }


//------------------------------------------------------------------------------
//  Flow* FlowManager::create()
//------------------------------------------------------------------------------
Flow FlowManager::create(const Flow &original) const
{
    TRACE("FlowManager::create2 -->");
    Flow result(original);
    result.topology = topology;
    TRACE("FlowManager::create2 <--");
    return result;
}



