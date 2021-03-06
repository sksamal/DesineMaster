#ifndef BCUBEALGORITHM_H
#define BCUBEALGORITHM_H
/******************************************************************************

    header file BCubeAlgorithm.h for class: BCubeAlgorithm

    This class implements the base-class Algorithm for a BCube
    Routing implementation. Adapted based on other algorithms
    June 20, 2016 : Moded LevelPath and deque to .h from .cpp file

******************************************************************************/


// Include(s)
#include "Algorithms/Algorithm.h"
#include "Network/Flow.h"
#include "Network/Path.h"
#include "Network/Topology.h"
#include "RandomVariables/RandomNumberGenerator.h"
#include "Utils/Types.h"
using Types::TString;


class BCubeAlgorithm : public Algorithm
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
//  constructor BCubeAlgorithm();
//
//  Constructs BCubeAlgorithm with a specified topology and parameters
//  Input :
//  Output:
//
//------------------------------------------------------------------------------
   BCubeAlgorithm(const TString &args);


//------------------------------------------------------------------------------
//
//  destructor ~BCubeAlgorithm()
//  Input :
//  Output:
//
//------------------------------------------------------------------------------
    virtual ~BCubeAlgorithm();


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
#endif //DIJKSTRAALGORITHM_H
