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
namespace cma_legacy_qx_parser
{
string sqlInsertStringMaker(const cma_frame_legacy& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_qx (CDID, AcquisitionTime, WindSpeed, WindDirection,Air_Temperature,Humidity,Air_Pressure,Precipitation_Intensity,Radiation_Intensity,Daily_Rainfall,Battery_Voltage,Operation_Temperature) VALUES");


    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("WindSpeed");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("WindDirection");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Air_Temperature");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Humidity");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Air_Pressure");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Precipitation_Intensity");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Radiation_Intensity");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Daily_Rainfall");
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
	uint16_t windSpeed = makeword(data[6], data[7]);
	uint16_t windDirection = makeword(data[8], data[9]);
	uint16_t air_Temperature = makeword(data[10], data[11]);
	uint16_t humidity = makeword(data[12], data[13]);
	uint16_t air_Pressure= makeword(data[14], data[15]);
	uint16_t precipitation_Intensity= makeword(data[16], data[17]);
	uint16_t radiation_Intensity = makeword(data[18], data[19]);
	uint16_t daily_Rainfall = makeword(data[20], data[21]);
	uint16_t battery_Voltage = makeword(data[22], data[23]);
	uint16_t operation_Temperature = makeword(data[24], data[25]);

	uint16_t reserve1 = makeword(data[26], data[27]);
	uint16_t reserve2 = makeword(data[28], data[29]);

    	try
    	{
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sWindSpeed = boost::lexical_cast<string>(windSpeed);
		string sWindDirection = boost::lexical_cast<string>(windDirection);
		string sAir_Temperature= boost::lexical_cast<string>(air_Temperature);
		string sHumidity = boost::lexical_cast<string>(humidity);
		string sAir_Pressure = boost::lexical_cast<string>(air_Pressure);
		string sPrecipitation_Intensity = boost::lexical_cast<string>(precipitation_Intensity);
		string sRadiation_Intensity = boost::lexical_cast<string>(radiation_Intensity);
		string sDaily_Rainfall = boost::lexical_cast<string>(daily_Rainfall);
		string sBattery_Voltage = boost::lexical_cast<string>(battery_Voltage);
		string sOperation_Temperature = boost::lexical_cast<string>(operation_Temperature);


		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("WindSpeed",sWindSpeed));
		ret.insert(make_pair<string,string>("WindDirection",sWindDirection));
		ret.insert(make_pair<string,string>("Air_Temperature",sAir_Temperature));
		ret.insert(make_pair<string,string>("Humidity",sHumidity));
		ret.insert(make_pair<string,string>("Air_Pressure",sAir_Pressure));
		ret.insert(make_pair<string,string>("Precipitation_Intensity",sPrecipitation_Intensity));
		ret.insert(make_pair<string,string>("Radiation_Intensity",sRadiation_Intensity));

		ret.insert(make_pair<string,string>("Daily_Rainfall",sDaily_Rainfall));
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
