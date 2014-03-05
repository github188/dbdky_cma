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
namespace cma_dxhc_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_dxhc (CDID, AcquisitionTime, Conductor_Sag, Toground_Distance,Angle,Measure_Flag) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Conductor_Sag");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Toground_Distance");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Angle");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Measure_Flag");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    

    ret += "')";

    return ret;
}

map<string,string> frameParserFunc(cma_frame& frm)
{
   	map<string,string> ret;
    	char*  componentId[17];
    	const uint8_t* data = const_cast<cma_frame*>(&frm)->getPduData();
    	memcpy(componentId, data, 17);

    	uint32_t timestamp = makelong(makeword(data[17], data[18]), makeword(data[19], data[20]));
	uint16_t alertflag = makeword(data[21], data[22]);
	uint32_t conductor_Sag = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));
	uint32_t toground_Distance = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));
	uint32_t angle = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));
	uint8_t measure_Flag = data[35];
	
	uint32_t reserve1 = makelong(makeword(data[36], data[37]), makeword(data[38], data[39]));
	uint32_t reserve2 = makelong(makeword(data[40], data[41]), makeword(data[42], data[43]));


    	try
    	{
    	//    string sTimestamp = boost::lexical_cast<string>(timestamp);
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sConductor_Sag = boost::lexical_cast<string>(conductor_Sag);
		string sToground_Distance = boost::lexical_cast<string>(toground_Distance);
		string sAngle = boost::lexical_cast<string>(angle);
		string sMeasure_Flag = boost::lexical_cast<string>(measure_Flag);

		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Conductor_Sag",sConductor_Sag));
		ret.insert(make_pair<string,string>("Toground_Distance",sToground_Distance));
		ret.insert(make_pair<string,string>("Angle",sAngle));
		ret.insert(make_pair<string,string>("Measure_Flag",sMeasure_Flag));
		
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
