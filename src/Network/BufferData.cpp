/*
 * BufferData.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: suraj
 */
#ifndef SRC_NETWORK_BUFFERDATA_CPP_
#define SRC_NETWORK_BUFFERDATA_CPP_

#include "BufferData.h"
#include "Utils/TraceManager.h"

#include <ostream>
#include <iomanip>
using std::setw;


void BufferData::create( int &id,
		 int &source,
		 int &dest,
		 double &tstart,
		 double &tend,
		 double &cap)
{
    TRACE("BufferData::BufferData -->");

    this->id = id;
    this->source = source;
    this->dest = dest;
    this->tstart = tstart;
    this->tend = tend;
    this->cap = cap;
    this->flow = 1;
    TRACE("BufferData::BufferData <--");
}

BufferData::~BufferData()
{
    TRACE("BufferData::~BufferData -->");
    // Empty
    TRACE("BufferData::~BufferData <--");
}

void BufferData::print(ostream &buffer) const
{
    buffer
    << "Flow> ID: " << setw(6) << id
    << "  src: "    << setw(4) << source
    << "  dst: "    << setw(4) << dest
    << "  Cap: " 	<< setw(8) << cap
    << "  Tstart: " << setw(8) << tstart
    << "  Tend: "   << setw(8) << tend;

    buffer << endl;
}

#endif

