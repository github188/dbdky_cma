#include "cma_callbacks.h"

#include <map>
#include <string>
#include <utility>

#include "cma_frame.h"
#include <string.h>

#include <utils/Logging.h>
#include <utils/Timestamp.h>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <util.h>

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_zdtz_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_zdtz (CDID, AcquisitionTime, Vibration_Amplitude, Vibration_Frequency) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Vibration_Amplitude");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Vibration_Frequency");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;


    ret += "')";

    return ret;
}

map<string,string> frameParserFunc(cma_frame& frm)
{
   	map<string,string> ret;
    //Chen Hongquan modify, begin
    	//char*  componentId[17];
    char componentId[17];
    //Chen Hongquan modify, end
    	const uint8_t* data = const_cast<cma_frame*>(&frm)->getPduData();
    	memcpy(componentId, data, 17);

    	uint32_t timestamp = makelong(makeword(data[17], data[18]), makeword(data[19], data[20]));
	uint16_t alertflag = makeword(data[21], data[22]);
	uint16_t vibration_Amplitude = makeword(data[23], data[24]);
	uint32_t vibration_Frequency = makelong(makeword(data[25], data[26]), makeword(data[27], data[28]));
	
	uint32_t reserve1 = makelong(makeword(data[29], data[30]), makeword(data[31], data[32]));
	uint32_t reserve2 = makelong(makeword(data[33], data[34]), makeword(data[35], data[36]));

    	try
    	{
    	//    string sTimestamp = boost::lexical_cast<string>(timestamp);
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sVibration_Amplitude = boost::lexical_cast<string>(vibration_Amplitude);
		string sVibration_Frequency = boost::lexical_cast<string>(vibration_Frequency);
		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Vibration_Amplitude",sVibration_Amplitude));
		ret.insert(make_pair<string,string>("Vibration_Frequency",sVibration_Frequency));
		
		ret.insert(make_pair<string,string>("Reverse1",sReverse1));
		ret.insert(make_pair<string,string>("Reverse2",sReverse2));
	
    	}
    	catch (boost::bad_lexical_cast& e)
    	{
        	return ret;
    	}

    	return ret;
}
}
}
