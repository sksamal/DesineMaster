/*
 * BufferList.h
 *
 *  Created on: Mar 11, 2016
 *      Author: suraj
 */

#ifndef SRC_NETWORK_BUFFERLIST_H_
#define SRC_NETWORK_BUFFERLIST_H_

using std::multimap;
using std::ostream;
#include "Utils/TraceManager.h"
#include "Network/BufferData.h"

class BufferList{

public:
    class Iterator;

    // Friends
    friend class FlowManager;

    // Typedef(s)
    private:
    typedef multimap<double, BufferData> BufferMap;

    // Variable(s)
    private:
    // Flows are stored in a multimap. The Tend values are used as 'key' so that
    // the (multi)map automatically stores the flows in the correct order
    BufferMap buffer_map;
    Time* time;

    public:

    ~BufferList();


    void accept( BufferData &buf);


    Iterator getIterator() const;

    void print(ostream &buffer) const;


    int size() const
       {
           return buffer_map.size();
       }

    void setTime(Time* t)
        {
            TRACE("FlowList::setTime-->");
            time = t;
            TRACE("FlowList::setTime<--");
        }

    bool update(const double &global_time);

    bool exists(int id);

    bool addCap(int id, double cap);

    BufferData getBuffer(int id);

    void erase(double key)
    {
    	buffer_map.erase(key);

    }

    BufferData& getFirstBuf()
    {
    	BufferMap::iterator iter = buffer_map.begin();
    	return iter->second;
    }

    public:
        class Iterator
        {
            friend class BufferList;
            private:
            // Typedef(s)
            //typedef FlowMap::const_iterator FlowMapIter;

            // Variable(s)
            BufferMap::const_iterator buf_iter;
            BufferMap::const_iterator buf_iter_end;

            // Function(s) declarations
            private:
        //--------------------------------------------------------------------------
        //
        //  constructor Iterator(multimap<double, Flow*>* flowmap)
        //
        //  Constructs Iterator for all flows
        //  Input : the link list
        //  Output:
        //--------------------------------------------------------------------------
            Iterator(const BufferMap &bufmap)
            {
            	buf_iter = bufmap.begin();
            	buf_iter_end = bufmap.end();
            }


            public:
        //--------------------------------------------------------------------------
        //
        //  destructor ~Iterator()
        //  Input :
        //  Output:
        //
        //--------------------------------------------------------------------------
            ~Iterator() {}


        //--------------------------------------------------------------------------
        //  void operator++();
        //
        //  Increment the iterator
        //  Input :
        //  Output:
        //--------------------------------------------------------------------------
            void operator++()
            {
                ++buf_iter;
            }


        //--------------------------------------------------------------------------
        //  bool operator()();
        //
        //  Checks if end has been reached of iterator
        //  Input :
        //  Output: true if more elements
        //--------------------------------------------------------------------------
            bool operator()() const
            {
                return buf_iter != buf_iter_end;
            }


        //--------------------------------------------------------------------------
        //  Link* operator*();
        //
        //  Returns the actual element (the Link)
        //  Input :
        //  Output: the link
        //--------------------------------------------------------------------------
            const BufferData& operator*() const
            {
                return buf_iter->second;
            }
        };

};


#endif /* SRC_NETWORK_BUFFERLIST_H_ */
