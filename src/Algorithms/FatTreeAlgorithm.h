#ifndef FATTREEALGORITHM_H
#define FATTREEALGORITHM_H
/******************************************************************************

    header file FatTreeAlgorithm.h for class: FatTreeAlgorithm

    This class implements the base-class Algorithm for a FatTree 
    implementation. It defines two distance functions: one is
    based on link metric, the other is the maximal reservable bandwidth (mrb)
    of the path. When selecting the next node to mark, one selects the node
    with the minimal length. If several nodes have the same minimal length, one
    selects the node with the largest mrb.


******************************************************************************/


// Include(s)
#include "Algorithms/Algorithm.h"
#include "Network/Flow.h"
#include "Network/Path.h"
#include "Network/Topology.h"
#include "RandomVariables/RandomNumberGenerator.h"
#include "Utils/Types.h"
using Types::TString;


class FatTreeAlgorithm : public Algorithm
{
    // Algorithm Factory is a concrete factory that is solely allowed to
    // instantiate new Algorithm subclasses
    friend class AlgorithmFactory;

    // Constanst(s)

    // Variable(s)
    private:
    RandomNumberGenerator* rng;

    // Function(s) declarations
    public:
//------------------------------------------------------------------------------
//
//  constructor FatTreeAlgorithm();
//
//  Constructs FatTreeAlgorithm with a specified topology and parameters
//  Input :
//  Output:
//
//------------------------------------------------------------------------------
    FatTreeAlgorithm(const TString &args);


//------------------------------------------------------------------------------
//
//  destructor ~FatTreeAlgorithm()
//  Input :
//  Output:
//
//------------------------------------------------------------------------------
    ~FatTreeAlgorithm();


//------------------------------------------------------------------------------
//  Path compute(const Flow &flow)
//
//  Computes the Path for a given flow according to the implementation of the
//  algorithm.
//  Input : the flow for which to find a path
//  Output:
//      - : the calculated path
//------------------------------------------------------------------------------
    Path compute(const Flow &flow)
    {
        return compute(flow.getSource(), flow.getDestination(),
                       flow.getTopology());
    }


//------------------------------------------------------------------------------
//  Path compute(const int source, int destination)
//
//  Computes the Path for a given source and destination pair
//  Input : s & d
//  Output:
//      - : the calculated path
//------------------------------------------------------------------------------
    Path compute(const int &flow_source, const int &flow_destination,
                 Topology* topology);


}; //end declarations
#endif //WSDIJKSTRAALGORITHM_H
