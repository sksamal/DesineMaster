/******************************************************************************

    header file TopologyFactory.h for class: TopologyFactory

    This class is a factory for creating Topology objects. It follows the
    concrete factory design pattern. The arguments that define the topology
    to be built, are passed on as a string. The Factory then parses the string
    and constructs the object likewise.

    Author : T. Kleiberg
    Version: 1
    Date   : Sep 2006

******************************************************************************/


// Include(s)
#include "Network/Topology.h"
#include "RandomVariables/RandomNumberGenerator.h"
#include "RandomVariables/RandomVar.h"
#include "Utils/Types.h"

// Print BCube Address 
    string FormattedBCubeAdd(int *addr,int level); 

// Generate Address in reverse order - used for efficiency in BCube
// Decimal to X base
   void genBCubeAddress(int number, int X, int *address, int length); 

