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
namespace cma_gt_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_gtqx (CDID, AcquisitionTime, Inclination, Inclination_X,Inclination_Y,Angle_X,Angle_Y) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Inclination");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Inclination_X");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Inclination_Y");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Angle_X");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Angle_Y");
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
	uint32_t inclination = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));
	uint32_t inclination_X = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));
	uint32_t inclination_Y = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));
	uint32_t angle_X = makelong(makeword(data[35], data[36]), makeword(data[37], data[38]));
	uint32_t angle_Y = makelong(makeword(data[39], data[40]), makeword(data[41], data[42]));

	uint32_t reserve1 = makelong(makeword(data[43], data[44]), makeword(data[45], data[46]));
	uint32_t reserve2 = makelong(makeword(data[47], data[48]), makeword(data[49], data[50]));




    	try
    	{
    	//    string sTimestamp = boost::lexical_cast<string>(timestamp);
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sInclination = boost::lexical_cast<string>(inclination);
		string sInclination_X = boost::lexical_cast<string>(inclination_X);
		string sInclination_Y = boost::lexical_cast<string>(inclination_Y);
		string sAngle_X = boost::lexical_cast<string>(angle_X);
		string sAngle_Y = boost::lexical_cast<string>(angle_Y);
		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Inclination",sInclination));
		ret.insert(make_pair<string,string>("Inclination_X",sInclination_X));
		ret.insert(make_pair<string,string>("Inclination_Y",sInclination_Y));
		ret.insert(make_pair<string,string>("Angle_X",sAngle_X));
		ret.insert(make_pair<string,string>("Angle_Y",sAngle_Y));
		
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
