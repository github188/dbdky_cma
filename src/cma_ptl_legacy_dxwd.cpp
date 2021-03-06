#include "cma_callbacks.h"

#include <map>
#include <string>
#include <utility>

#include "cma_frame_legacy.h"
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
using dbdky::cma_server::cma_frame_legacy;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_legacy_dxwd_parser
{
string sqlInsertStringMaker(const cma_frame_legacy& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_dxwd (CDID, AcquisitionTime, Line_Temperature1, Line_Temperature2,Battery_Voltage,Operation_Temperature) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Line_Temperature1");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Line_Temperature2");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";
    
    itr = mp.find("Battery_Voltage");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Operation_Temperature");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;

    ret += "')";

    return ret;
}

map<string,string> frameParserFunc(cma_frame_legacy& frm)
{
   	map<string,string> ret;
    	char*  sensor_ID[2];
    	const uint8_t* data = const_cast<cma_frame_legacy*>(&frm)->getPduData();
    	memcpy(sensor_ID, data, 2);

    	uint32_t timestamp = makelong(makeword(data[2], data[3]), makeword(data[4], data[5]));
	uint16_t line_Temperature1 = makeword(data[6], data[7]);
	uint16_t line_Temperature2 = makeword(data[8], data[9]);
	uint16_t battery_Voltage = makeword(data[10], data[11]);
	uint16_t operation_Temperature = makeword(data[12], data[13]);

	uint32_t reserve1 = makeword(data[14], data[15]);
	uint32_t reserve2 = makeword(data[16], data[17]);

    	try
    	{
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sLine_Temperature1 = boost::lexical_cast<string>(line_Temperature1);
		string sLine_Temperature2 = boost::lexical_cast<string>(line_Temperature2);
		string sBattery_Voltage = boost::lexical_cast<string>(battery_Voltage);
		string sOperation_Temperature = boost::lexical_cast<string>(operation_Temperature);
		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);

		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Line_Temperature1",sLine_Temperature1));
		ret.insert(make_pair<string,string>("Line_Temperature2",sLine_Temperature2));
		ret.insert(make_pair<string,string>("Battery_Voltage",sBattery_Voltage));
		ret.insert(make_pair<string,string>("Operation_Temperature",sOperation_Temperature));
		
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
