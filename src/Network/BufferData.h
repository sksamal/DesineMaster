/*
 * BufferData.h
 *
 *  Created on: Mar 11, 2016
 *      Author: suraj
 */

#ifndef SRC_NETWORK_BUFFERDATA_H_
#define SRC_NETWORK_BUFFERDATA_H_

#include <ostream>
using std::ostream;

class BufferData
{
	friend class FlowManager;

private:
	int id;                 // flow id
	int source;             // source node
	int dest;               // destination node
	double tstart;          // flow starting time
	double tend;            // flow ending time
	double cap;
	int flow;

public:
	void create (int &id,
			 int &source,
			 int &dest,
			 double &tstart,
			 double &tend,
			 double &cap);
public:
	~BufferData();

	void print(ostream &buffer) const;

		int getId() const { return id; }
		int getSource() const { return source; }
		int getDestination() const { return dest; }
		double getDuration() const { return (tend-tstart); }
		double getRequestedCapacity() const { return cap; }
		double getTstart() const { return tstart; }
		double getTend() const { return tend; }
		void updateCap(double &capacity) { this->cap+=capacity;}
		void incFlowNo() { flow++; }
		int getFlows() { return flow;}

};


#endif /* SRC_NETWORK_BUFFERDATA_H_ */
