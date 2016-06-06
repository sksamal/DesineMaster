/*
 * BufferList.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: suraj
 */
#ifndef SRC_NETWORK_BUFFERLIST_CPP_
#define SRC_NETWORK_BUFFERLIST_CPP_

#include "Network/BufferList.h"
#include "Utils/TraceManager.h"
#include <ostream>
using Types::uli;
using std::endl;
using std::ostream;

BufferList::~BufferList()
{
    TRACE("BufferList::~BufferList -->");
    buffer_map.clear();
    TRACE("BufferList::~BufferList <--");
}


void BufferList::accept(BufferData &buf)
{
	TRACE("BufferList::accept -->");

    buffer_map.insert(std::make_pair(buf.getTstart(), BufferData(buf)));

	TRACE("BufferList::accept <--");
}


BufferList::Iterator BufferList::getIterator() const
{
    TRACE("BufferList::getIterator -->");

    BufferList::Iterator result(buffer_map);

    TRACE("BufferList::getIterator <--");
    return result;
}

void BufferList::print(ostream &buffer) const
{
    buffer << "BufferList> Size: " << buffer_map.size() << endl;
    for (BufferMap::const_iterator iter = buffer_map.begin();
         iter != buffer_map.end(); ++iter)
    {
        iter->second.print(buffer);
    }
}

bool BufferList::update(const double &global_time)
{
    TRACE("BufferList::update -->");

    BufferMap::iterator bound = buffer_map.upper_bound(global_time);
    BufferMap::iterator iter = buffer_map.begin();

    if (iter!=bound)
    	return true;

    else
    	return false;

    TRACE("BufferList::update <--");
}

bool BufferList::exists(int id)
{
    for (BufferMap::const_iterator iter = buffer_map.begin();
         iter != buffer_map.end(); ++iter)
    {
        if(iter->second.getId()==id)
        return true;
    }
    return false;
}

bool BufferList::addCap(int id, double cap)
{
	for (BufferMap::iterator iter = buffer_map.begin();
	         iter != buffer_map.end(); ++iter)
	    {
			int id1=iter->second.getId();
	        if(id1==id)
	        {
	        	cap+=iter->second.getRequestedCapacity();
			iter->second.updateCap(cap);
			iter->second.incFlowNo();
			
	        	if(iter->second.getRequestedCapacity() > 20)
	        	return true;

	        	else
	        		return false;
	        }
	    }
	return false;
}

BufferData BufferList::getBuffer(int id)
{
	for (BufferMap::const_iterator iter = buffer_map.begin();
		         iter != buffer_map.end(); ++iter)
		    {
		        if(iter->second.getId()==id)
		        return iter->second;
		    }


}

#endif

