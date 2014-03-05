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
namespace cma_wdtz_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_wdtz (CDID, AcquisitionTime, U_Gallop_Amplitude,U_Vertical_Amplitude,U_Horizontal_Amplitude,U_AngleToVertical,U_Gallop_Frequency) VALUES");


    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("U_Gallop_Amplitude");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("U_Vertical_Amplitude");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("U_Horizontal_Amplitude");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("U_AngleToVertical");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("U_Gallop_Frequency");
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
	uint32_t U_Gallop_Amplitude = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));
	uint32_t U_Vertical_Amplitude = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));
	uint32_t U_Horizontal_Amplitude = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));
	uint32_t U_AngleToVertical = makelong(makeword(data[35], data[36]), makeword(data[37], data[38]));
	uint32_t U_Gallop_Frequency = makelong(makeword(data[39], data[40]), makeword(data[41], data[42]));
	
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
		string sU_Gallop_Amplitude = boost::lexical_cast<string>(U_Gallop_Amplitude);
		string sU_Vertical_Amplitude = boost::lexical_cast<string>(U_Vertical_Amplitude);
		string sU_Horizontal_Amplitude = boost::lexical_cast<string>(U_Horizontal_Amplitude);
		string sU_AngleToVertical = boost::lexical_cast<string>(U_AngleToVertical);
		string sU_Gallop_Frequency = boost::lexical_cast<string>(U_Gallop_Frequency);

		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("U_Gallop_Amplitude",sU_Gallop_Amplitude));
		ret.insert(make_pair<string,string>("U_Vertical_Amplitude",sU_Vertical_Amplitude));
		ret.insert(make_pair<string,string>("U_Horizontal_Amplitude",sU_Horizontal_Amplitude));
		ret.insert(make_pair<string,string>("U_AngleToVertical",sU_AngleToVertical));
		ret.insert(make_pair<string,string>("U_Gallop_Frequency",sU_Gallop_Frequency));
		
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
