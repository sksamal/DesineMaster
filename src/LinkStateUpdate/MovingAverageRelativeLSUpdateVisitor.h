#ifndef MOVINGAVERAGERELATIVELSUPDATEVISITOR_H
#define MOVINGAVERAGERELATIVELSUPDATEVISITOR_H

/******************************************************************************

    header file MovingAverageRelativeLSUpdateVisitor.h for class:
    MovingAverageRelativeLSUpdateVisitor

    This class is meant to represent an instance of ANEV for link-state update


    Author : Bingjie FU
    Version: 1
    Date   : Jan 2005

******************************************************************************/

#include "LinkStateUpdate/LinkStateUpdateVisitor.h"
#include "Utils/TraceManager.h"


class MovingAverageRelativeLSUpdateVisitor : public LinkStateUpdateVisitor
{
    private:
    const int window_size;         // may be got from parameters in the future
    const double threshold;   // may be got from parameters in the future
    // the function used to check if the condition of update is met
    // and if so return the new updated_value
    // if not return the old one

    double new_value_from_update;
    double current_known_capacity;
    double last_N_average;


    public:
//------------------------------------------------------------------------------
//
//  constructor MovingAverageLSUpdateVisitor();
//
//  Constructs MovingAverageLSUpdateVisitor. Only used by ..Factory
//
//------------------------------------------------------------------------------
    MovingAverageRelativeLSUpdateVisitor(
    	const int &window_size,
    	const double &threshold,
    	const bool &is_parent = true);


//------------------------------------------------------------------------------
//  double getLSUpdateCapacity();
//
//  GEt the ....
//
//------------------------------------------------------------------------------
    double getLSUpdateCapacity()
    {
        return new_value_from_update;
    }


//------------------------------------------------------------------------------
//  void updateLSCapacity()
//
//  Calculate the new Link State Update Capacity value according to the
//  update policy
//
//------------------------------------------------------------------------------
    void updateLSCapacity();


//------------------------------------------------------------------------------
//  void visitLink(const AbstractNetworkElement& link)
//
//  See AbstractNetworkElementVisitor for detais
//
//------------------------------------------------------------------------------
    void visitLink(const AbstractNetworkElement* link);

//------------------------------------------------------------------------------
//  uli getUpdateNum()
//
//  get the number of updates that happened
//  if it is the main visitor,then add all the update numbers from child visitor
//  if it is a child visitor, just return the number of updates happened on its link
//
//------------------------------------------------------------------------------
    uli getUpdateNum();


//------------------------------------------------------------------------------
//  void reset()
//  reset the visitor
//
//------------------------------------------------------------------------------
    void reset();

//------------------------------------------------------------------------------
//  void print()
//  print the visitor
//
//------------------------------------------------------------------------------
    void print(ostream &buffer);

};
#endif //MOVINGAVERAGERELATIVELSUPDATEVISITOR_H
