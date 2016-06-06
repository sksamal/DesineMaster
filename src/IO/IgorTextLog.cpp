/******************************************************************************

	source file IgorTextLog.cpp for class: IgorTextLog

	Author : T. Kleiberg
	Version: 1
	Date   : Feb 2005

******************************************************************************/


// Include(s)
#include "IO/IgorTextLog.h"
#include "IO/WaveSet.h"
#include "Utils/Types.h"
#include <fstream>
#include <sstream>
#include <string>
using std::endl;
using std::ofstream;
using std::string;
using Types::uli;

//------------------------------------------------------------------------------
//  constructor IgorTextLog()
//------------------------------------------------------------------------------
IgorTextLog::IgorTextLog(const string &name, const string &path)
	: AbstractTextLog(name, path)
{
	TRACE("IgorTextLog::IgorTextLog -->");
	txtname = path + "/" + name + ".txt";
	TRACE("IgorTextLog::IgorTextLog <--");
}

//------------------------------------------------------------------------------
//  destructor ~IgorTextLog()
//------------------------------------------------------------------------------
IgorTextLog::~IgorTextLog()
{
	TRACE("IgorTextLog::~IgorTextLog -->");
	if (txtbuffer.is_open()) close();
	TRACE("IgorTextLog::~IgorTextLog <--");
}


//------------------------------------------------------------------------------
//  void append();
//------------------------------------------------------------------------------
void IgorTextLog::close()
{
	TRACE("IgorTextLog::close -->");

	// Write names
//	print(txtbuffer, stringSet, -1);
//	print(txtbuffer, doubleSet, -1);
//	print(txtbuffer, longSet, -1);
//	print(txtbuffer, uliSet, -1);
//	txtbuffer << endl;

	// Write values until no ..Set contains anymore values (index out of range)
	bool indexInRange = true;
	for(long index = 0; indexInRange ;++index)
	{
		indexInRange =  printAll(txtbuffer, index);
//		indexInRange |= print(txtbuffer, doubleSet, index);
//		indexInRange |= print(txtbuffer, longSet, index);
//		indexInRange |= print(txtbuffer, uliSet, index);
		if (indexInRange) txtbuffer << endl;
	}
	txtbuffer.close();
	TRACE("IgorTextLog::close <--");
}


//------------------------------------------------------------------------------
//  void append();
//------------------------------------------------------------------------------
void IgorTextLog::open()
{
	TRACE("IgorTextLog::open -->");
	// All writing is done in close(). This is done because not all
	// waves are known at this point yet
	txtbuffer.open(txtname.c_str(), ofstream::out | ofstream::trunc);

	TRACE("IgorTextLog::open <--");
}


//------------------------------------------------------------------------------
//  void printAll();
//------------------------------------------------------------------------------
bool IgorTextLog::printAll(ostream &buffer, const long &index) const
{
	const vector<WSstring>& ws1 = stringSet;
	const vector<WSdouble>& ws2 = doubleSet;
	typedef typename vector<WSstring>::const_iterator VWSIterator1;
	typedef WSstring::Iterator WaveIter1;
	typedef typename vector<WSdouble>::const_iterator VWSIterator2;
	typedef WSdouble::Iterator WaveIter2;

	// Iterate through vector of wave sets
	std::stringstream tmpbuf;
	VWSIterator1 iter11;
	VWSIterator2 iter12;
	for(iter11 = ws1.begin(),iter12 = ws2.begin();iter11 != ws1.end(),iter12 != ws2.end(); ++iter11,++iter12)
	{
		// Iterate though waves in a wave set
		WaveIter2 iter22(iter12->getWaves());
		for (;iter22(); ++iter22)
		{
			// Names must be written
			if ((uli) index  < iter22.getWaveVector().size())
			{
				tmpbuf
				<< (tmpbuf.str().empty() ? "" : "\n")
				<< fixformat(iter11->getName())
				<< (iter11->getName().length() ? "_" : "")
				<< fixformat(iter22.getName())
				<< "\t"
				<< iter22.getWaveVector()[index];
			}
		}
	}
	if (!tmpbuf.str().empty())
	{
		buffer << tmpbuf.str();
	}
	return (!tmpbuf.str().empty());
}

//
// print
//------------------------------------------------------------------------------
template <typename T>
bool IgorTextLog::print(ostream &buffer, const vector<T>& ws, const long &index) const
{
	typedef typename vector<T>::const_iterator VWSIterator;
	typedef typename T::Iterator WaveIter;

	// Iterate through vector of wave sets
	std::stringstream tmpbuf;
	for(VWSIterator iter1 = ws.begin(); iter1 != ws.end(); ++iter1)
	{
		// Iterate though waves in a wave set
		for (WaveIter iter2 = iter1->getWaves(); iter2(); ++iter2)
		{
			// Check if names must be written
			if (index < 0) // index < 0 -> print names as header line
			{
				tmpbuf
				<< (tmpbuf.str().empty() ? "" : "\t")
				<< fixformat(iter1->getName())
				<< (iter1->getName().length() ? "_" : "")
				<< fixformat(iter2.getName());
			} else
			if ((uli) index  < iter2.getWaveVector().size())
			{
				tmpbuf
				<< (tmpbuf.str().empty() ? "" : "\t")
				<< iter2.getWaveVector()[index];
			}
		}
	}
	if (!tmpbuf.str().empty())
	{
		buffer << tmpbuf.str();
	}
	return (!tmpbuf.str().empty());
}
