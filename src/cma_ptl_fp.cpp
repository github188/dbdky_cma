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
namespace cma_fp_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_fp (CDID, AcquisitionTime, Windage_Yaw_Angle, Deflection_Angle,Least_Clearance) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Windage_Yaw_Angle");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Deflection_Angle");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Least_Clearance");
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
	uint32_t windage_Yaw_Angle = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));
	uint32_t deflection_Angle = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));
	uint32_t least_Clearance = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));
	
	uint32_t reserve1 = makelong(makeword(data[35], data[36]), makeword(data[37], data[38]));
	uint32_t reserve2 = makelong(makeword(data[39], data[40]), makeword(data[41], data[42]));

    	try
    	{
    	//    string sTimestamp = boost::lexical_cast<string>(timestamp);
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sWindage_Yaw_Angle = boost::lexical_cast<string>(windage_Yaw_Angle);
		string sDeflection_Angle = boost::lexical_cast<string>(deflection_Angle);
		string sLeast_Clearance = boost::lexical_cast<string>(least_Clearance);

		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Windage_Yaw_Angle",sWindage_Yaw_Angle));
		ret.insert(make_pair<string,string>("Deflection_Angle",sDeflection_Angle));
		ret.insert(make_pair<string,string>("Least_Clearance",sLeast_Clearance));
		
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
