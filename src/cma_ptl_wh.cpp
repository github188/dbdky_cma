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
#include <Hex2FloatConvert.h>			//xinsy20140324

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_wh_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_jyzwh (CDID, AcquisitionTime, ESDD, NSDD,Daily_Max_Temperature,Daily_Min_Temperature,Daily_Max_Humidity,Daily_Min_Humidity) VALUES");


    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("ESDD");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("NSDD");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Daily_Max_Temperature");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Daily_Min_Temperature");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Daily_Max_Humidity");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Daily_Min_Humidity");
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
	uint32_t ESDD_tmp = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));	//xinsy20140324
	float ESDD= getDecNumber(ESDD_tmp);								//xinsy20140324
	uint32_t NSDD_tmp = makelong(makeword(data[27], data[28]), makeword(data[29], data[30]));	//xinsy20140324
	float NSDD= getDecNumber(NSDD_tmp);								//xinsy20140324
	uint32_t daily_Max_Temperature_tmp = makelong(makeword(data[31], data[32]), makeword(data[33], data[34]));	//xinsy20140324
	float daily_Max_Temperature= getDecNumber(daily_Max_Temperature_tmp);				//xinsy20140324
	uint32_t daily_Min_Temperature_tmp = makelong(makeword(data[35], data[36]), makeword(data[37], data[38]));	//xinsy20140324
	float daily_Min_Temperature= getDecNumber(daily_Min_Temperature_tmp);				//xinsy20140324
	uint16_t daily_Max_Humidity = makeword(data[39], data[40]);
	uint16_t daily_Min_Humidity = makeword(data[41], data[42]);

	uint32_t reserve1 = makelong(makeword(data[43], data[44]), makeword(data[45], data[46]));
	uint32_t reserve2 = makelong(makeword(data[47], data[48]), makeword(data[49], data[50]));
	

    	try
    	{
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sESDD = boost::lexical_cast<string>(ESDD);
		string sNSDD = boost::lexical_cast<string>(NSDD);
		string sDaily_Max_Temperature = boost::lexical_cast<string>(daily_Max_Temperature);
		string sDaily_Min_Temperature = boost::lexical_cast<string>(daily_Min_Temperature);
		string sDaily_Max_Humidity = boost::lexical_cast<string>(daily_Max_Humidity);
		string sDaily_Min_Humidity = boost::lexical_cast<string>(daily_Min_Humidity);

		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);

	#if 1	//xinsy20140324
		LOG_INFO << "ESDD : " << sESDD;
		LOG_INFO << "NSDD : " << sNSDD;
		LOG_INFO << "Daily_Max_Temperature : " << sDaily_Max_Temperature;
		LOG_INFO << "Daily_Min_Temperature : " << sDaily_Min_Temperature;
		LOG_INFO << "Daily_Max_Humidity : " << sDaily_Max_Humidity;
		LOG_INFO << "Daily_Min_Humidity : " << sDaily_Min_Humidity;
	#endif 


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("ESDD",sESDD));
		ret.insert(make_pair<string,string>("NSDD",sNSDD));
		ret.insert(make_pair<string,string>("Daily_Max_Temperature",sDaily_Max_Temperature));
		ret.insert(make_pair<string,string>("Daily_Min_Temperature",sDaily_Min_Temperature));
		ret.insert(make_pair<string,string>("Daily_Max_Humidity",sDaily_Max_Humidity));
		ret.insert(make_pair<string,string>("Daily_Min_Humidity",sDaily_Min_Humidity));
		
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
