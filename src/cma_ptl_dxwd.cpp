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
#include <Hex2FloatConvert.h>					//xinsy20140326

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_dxwd_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
	map<string,string> mp = frm.getMoniDataMap();
	string ret("insert into sd_dxwd (CDID, AcquisitionTime, Line_Temperature1, Line_Temperature2) VALUES");

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

	uint32_t line_Temperature1_tmp = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));		//xinsy20140326
	float 	 line_Temperature1 = getDecNumber(line_Temperature1_tmp);						//xinsy20140326

	uint32_t line_Temperature2_tmp = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));		//xinsy20140326
	float 	 line_Temperature2 = getDecNumber(line_Temperature2_tmp);						//xinsy20140326

	uint32_t reserve1 = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));
	uint32_t reserve2 = makelong(makeword(data[35], data[36]), makeword(data[37], data[38]));

    	try
    	{
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sLine_Temperature1 = boost::lexical_cast<string>(line_Temperature1);
		string sLine_Temperature2 = boost::lexical_cast<string>(line_Temperature2);
		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


	#if 1	//xinsy20140326
		LOG_INFO << "Line_Temperature1 : " << sLine_Temperature1;
		LOG_INFO << "Line_Temperature2 : " << sLine_Temperature2;
	#endif 

		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Line_Temperature1",sLine_Temperature1));
		ret.insert(make_pair<string,string>("Line_Temperature2",sLine_Temperature2));
		
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
