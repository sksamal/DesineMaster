/******************************************************************************

    source file DesineSumData.cpp for class: DesineSumData

    See header file for details

    Author : T. Kleiberg
    Version: 1
    Date   : Sep 2004
    Feb8,2016: Merged changes
******************************************************************************/



// Include(s)
#include "DesineSumData.h"
#include "Utils/Types.h"
#include <fstream>
#include <iostream>
#include <string>
using std::endl;
using std::ostream;
using std::ofstream;
using std::string;
using Types::uli;

// Constanst(s)

// Variable(s)

// Function(s) definitions

//------------------------------------------------------------------------------
//  constructor DesineSumData::DesineSumData(char* input)
//------------------------------------------------------------------------------
DesineSumData::DesineSumData(const string &name) : Data(name)
{
    TRACE("DesineSumData::DesineSumData1 -->");
    //Empty
    TRACE("DesineSumData::DesineSumData1 <--");
}


//------------------------------------------------------------------------------
//  destructor DesineSumData::~DesineSumData()
//------------------------------------------------------------------------------
DesineSumData::~DesineSumData()
{
    TRACE("DesineSumData::~DesineSumData -->");
    //Empty
    TRACE("DesineSumData::~DesineSumData <--");
}


//------------------------------------------------------------------------------
//  void DesineSumData::print(ostream& buffer)
//------------------------------------------------------------------------------
void DesineSumData::print(ostream& buffer) const
{
    TRACE("DesineSumData::print -->");
    for(DesineDataVector::const_iterator iter = dataset.begin();
        iter != dataset.end(); ++iter)
    {
        iter->print(buffer);
        buffer << endl;
    }
    TRACE("DesineSumData::print <--");
}

//------------------------------------------------------------------------------
//  void DesineSumData::write(LogFactory* log)
//------------------------------------------------------------------------------
void DesineSumData::write(const LogFactory* logfact) const
{
    TRACE("DesineSumData::write -->");

    AbstractLog* log = logfact->createTextLog(name);
    AbstractLog::WSstringVector wnames;
    AbstractLog::WSdoubleVector wvectors[13];

    // Iterate through the vector with the WaveSets

    double fl_total, fl_accept, fl_block, fl_rej_setup, t_total, t_accept, t_block, t_rej_setup, upd_num, t_process, hop;

    for(DesineDataVector::const_iterator iter = dataset.begin();
        iter != dataset.end(); ++iter)
    {
        double flow_rej_qos = 0.0;

        // Calculate Avg. hopcount and timers
        double flow_a = iter->flow_accepted.getMean() +
                        iter->flow_rejected_setup.getMean() +
                        flow_rej_qos;
        double avghop = flow_a ? iter->hop_total.getMean()/flow_a : 0.0;

         fl_total=iter->flow_total.getMean();
            fl_accept=iter->flow_accepted.getMean();
            fl_block=iter->flow_blocked.getMean();
            fl_rej_setup=iter->flow_rejected_setup.getMean();
            t_total=iter->tp_total.getMean()/8;
            t_accept=iter->tp_accepted.getMean()/8;
            t_block=iter->tp_blocked.getMean()/8;
            t_rej_setup=iter->tp_rejected_setup.getMean()/8;
            upd_num=iter->update_num.getMean();
            t_process=iter->tp_process.getMean();
            hop = avghop;

        short n = 0;
        wnames.push_back(iter->getName());

        // Make sure the vector is large enough!!
        wvectors[n++].push_back(fl_total);

        wvectors[n++].push_back(fl_accept);

         wvectors[n++].push_back(fl_block);
        wvectors[n++].push_back(fl_rej_setup);
//        wvectors[n++].push_back(flow_rej_qos);
        wvectors[n++].push_back(t_total);
        wvectors[n++].push_back(t_accept);
        wvectors[n++].push_back(t_block);
        wvectors[n++].push_back(t_rej_setup);
//        wvectors[n++].push_back(iter->tp_rejected_qos.getMean());
        wvectors[n++].push_back(avghop);
        wvectors[n++].push_back(upd_num);
	wvectors[n++].push_back(t_process);
#ifndef NO_TIMER
        wvectors[n++].push_back(iter->time_comp.getMean());
#endif //NO_TIMER

    }
    AbstractLog::WSstring wavesString(name);
    AbstractLog::WSdouble wavesDouble(name);
    short n = 0;
    wavesString.insert("Algorithm", wnames);
    wavesDouble.insert("FlowTotal", wvectors[n++]);
    wavesDouble.insert("FlowAccepted", wvectors[n++]);
    wavesDouble.insert("FlowBlocked", wvectors[n++]);
    wavesDouble.insert("FlowRejectSetup", wvectors[n++]);
//    wavesDouble.insert("FlowRejectQoS", wvectors[n++]);
    wavesDouble.insert("Data Total", wvectors[n++]);
    wavesDouble.insert("Data Accepted", wvectors[n++]);
    //added for obs by suraj y.

    wavesDouble.insert("Data Blocked", wvectors[n++]);
    wavesDouble.insert("Data RejectSetup", wvectors[n++]);
//    wavesDouble.insert("TPRejectQoS", wvectors[n++]);
    wavesDouble.insert("AvgHopcount", wvectors[n++]);
    wavesDouble.insert("NumUpdates", wvectors[n++]);
    wavesDouble.insert("Processing Time",wvectors[n++]);	
#ifndef NO_TIMER
    wavesDouble.insert("Timer", wvectors[n++]);
#endif //NO_TIMER

    log->open();
    log->append(wavesString);
    log->append(wavesDouble);
    log->close();
    delete log;

    cout<<endl<<" Flow Total "<<fl_total<<endl;
    cout<<" FlowAccepted "<<fl_accept<<endl;
    cout<<" FlowBlocked "<<fl_block<<endl;
    cout<<" FlowRejectSetup "<<fl_rej_setup<<endl;
    cout<<" Data Total "<<t_total<<endl;
    cout<<" Data Accepted "<<t_accept<<endl;
    cout<<" Data Blocked "<<t_block<<endl;
    cout<<" Data RejectSetup "<<t_rej_setup<<endl;
    cout<<" NumUpdates "<<upd_num<<endl;
    cout<<" Processing Time "<<t_process<<endl;
    cout<<" AvgHopcount "<<hop<<endl;


    TRACE("DesineSumData::write <--");
}

//------------------------------------------------------------------------------
//  &Data DesineSumData::operator+=(const Data &data)
//------------------------------------------------------------------------------
Data& DesineSumData::operator+=(const Data &data)
{
    TRACE("DesineSumData::operator+= -->");

    dataset.push_back( DesineData( *(DesineData*) &data )  );
#ifndef NO_THROUGHPUT
    // Remove vectors with throughput to prevent memory overflow problems
    // These vectors are not needed anymore
    dataset.back().numflows.clear();
    dataset.back().throughput.clear();
#endif
    TRACE("DesineSumData::operator+= <--");
    return *this;
}



/*

    //int precision = buffer.precision(6);
    //buffer << "Accepted: "      << setw(8) << flow_accepted.getMean()
    //buffer.precision(precision);

    char tab = '\t';

    buffer <<
   "Algorithm" << tab
    "FlowTotal" << tab
    "FlowAccepted" << tab
    "FlowBlocked" << tab
    "FlowRejectedSetup" << tab
    "FlowRejectQoS" << tab
    "ThroughputTotal" << tab
    "ThroughputAccepted" << tab
    "ThroughputBlocked" << tab
    "ThroughputRejectedSetup" << tab
    "ThroughputRejectQoS" << tab
    "AvgHopcount" << tab
    "Timer(ms)" << endl;

    for(DesineDataVector::const_iterator iter = dataset->begin();
        iter != dataset->end(); ++iter)
    {
        DVuli flow_rej_qos;
        for (uli count = 0; count < iter->flow_rejected_qos.size(); ++count)
        {
            flow_rej_qos += flow_rejected_qos[count];
        }

        // Calculate Avg. hopcount and timers
        double flow_a = iter->flow_accepted.getMean() +
                        iter->flow_rejected_setup.getMean() +
                        iter->flow_rej_qos.getMean();
        double avghop = flow_a ? iter->hop_total.getMean()/flow_a : 0.0;

        buffer <<
        iter->getName() << tab
        iter->flow_total.getMean() << tab
        iter->flow_accepted.getMean() << tab
        iter->flow_blocked.getMean() << tab
        iter->flow_rejected_setup.getMean() << tab
        iter->flow_rej_qos.getMean() << tab
        iter->tp_total.getMean() << tab
        iter->tp_accepted.getMean() << tab
        iter->tp_blocked.getMean() << tab
        iter->tp_rejected_setup.getMean() << tab
        iter->tp_rejected_qos.getMean() << tab
        avghop << tab
        iter->time_comp.getMean() << endl;
    }
*/
