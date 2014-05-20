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
#include <Hex2FloatConvert.h>			//xinsy20140326

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_qx_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
	map<string,string> mp = frm.getMoniDataMap();

	string ret("insert into sd_qx (CDID, AcquisitionTime, Average_WindSpeed_10min, Average_WindDirection_10min,Max_WindSpeed,Extreme_WindSpeed,Standard_WindSpeed,Air_Temperature,Humidity,Air_Pressure,Precipitation,Precipitation_Intensity,Radiation_Intensity) VALUES");

	ret += "(";

	ret += "'";
	ret += frm.getCdid();
	ret += "',"; 

	map<string,string>::iterator itr;

	itr = mp.find("Time_Stamp");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";


	itr = mp.find("Average_WindSpeed_10min");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";

	itr = mp.find("Average_WindDirection_10min");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";

	itr = mp.find("Max_WindSpeed");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";

	itr = mp.find("Extreme_WindSpeed");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";

	itr = mp.find("Standard_WindSpeed");
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

	itr = mp.find("Precipitation");
	ret += "'";
	ret += (itr == mp.end()) ? "" : itr->second;
	ret += "',";

	itr = mp.find("Precipitation_Intensity");
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

	uint32_t average_WindSpeed_10min_tmp = makelong(makeword(data[23], data[24]), makeword(data[25], data[26]));	//xinsy20140326
	float 	 average_WindSpeed_10min = getDecNumber(average_WindSpeed_10min_tmp);					//xinsy20140326

	uint16_t average_WindDirection_10min = makeword(data[27], data[28]);

	uint32_t max_WindSpeed_tmp = makelong(makeword(data[29], data[30]), makeword(data[31], data[32]));		//xinsy20140326
	float 	 max_WindSpeed = getDecNumber(max_WindSpeed_tmp);							//xinsy20140326

	uint32_t extreme_WindSpeed_tmp = makelong(makeword(data[33], data[34]), makeword(data[35], data[36]));		//xinsy20140326
	float 	 extreme_WindSpeed = getDecNumber(extreme_WindSpeed_tmp);						//xinsy20140326

	uint32_t standard_WindSpeed_tmp = makelong(makeword(data[37], data[38]), makeword(data[39], data[40]));		//xinsy20140326
	float 	 standard_WindSpeed = getDecNumber(standard_WindSpeed_tmp);						//xinsy20140326

	uint32_t air_Temperature_tmp = makelong(makeword(data[41], data[42]), makeword(data[43], data[44]));		//xinsy20140326
	float 	 air_Temperature = getDecNumber(air_Temperature_tmp);							//xinsy20140326

	uint16_t humidity = makeword(data[45], data[46]);

	uint32_t air_Pressure_tmp= makelong(makeword(data[47], data[48]), makeword(data[49], data[50]));		//xinsy20140326
	float 	 air_Pressure = getDecNumber(air_Pressure_tmp);								//xinsy20140326

	uint32_t precipitation_tmp= makelong(makeword(data[51], data[52]), makeword(data[53], data[54]));		//xinsy20140326
	float 	 precipitation = getDecNumber(precipitation_tmp);							//xinsy20140326

	uint32_t precipitation_Intensity_tmp= makelong(makeword(data[55], data[56]), makeword(data[57], data[58]));	//xinsy20140326
	float 	 precipitation_Intensity = getDecNumber(precipitation_Intensity_tmp);					//xinsy20140326

	uint16_t radiation_Intensity = makeword(data[59], data[60]);

	uint32_t reserve1 = makelong(makeword(data[61], data[62]), makeword(data[63], data[64]));
	uint32_t reserve2 = makelong(makeword(data[65], data[66]), makeword(data[67], data[68]));

    	try
    	{
		uint64_t ms = timestamp;
		ms *= 1000;
		ms *= 1000;
		Timestamp ts(ms);
		string sTimestamp = ts.toFormattedStringDash(); 
		string sAlertflag = boost::lexical_cast<string>(alertflag);
		string sAverage_WindSpeed_10min = boost::lexical_cast<string>(average_WindSpeed_10min);
		string sAverage_WindDirection_10min= boost::lexical_cast<string>(average_WindDirection_10min);
		string sMax_WindSpeed = boost::lexical_cast<string>(max_WindSpeed);
		string sExtreme_WindSpeed = boost::lexical_cast<string>(extreme_WindSpeed);
		string sStandard_WindSpeed = boost::lexical_cast<string>(standard_WindSpeed);
		string sAir_Temperature = boost::lexical_cast<string>(air_Temperature);
		string sHumidity = boost::lexical_cast<string>(humidity);
		string sAir_Pressure = boost::lexical_cast<string>(air_Pressure);
		string sPrecipitation = boost::lexical_cast<string>(precipitation);
		string sPrecipitation_Intensity = boost::lexical_cast<string>(precipitation_Intensity);
		string sRadiation_Intensity = boost::lexical_cast<string>(radiation_Intensity);

		string sReverse1 = boost::lexical_cast<string>(reserve1);
		string sReverse2 = boost::lexical_cast<string>(reserve2);


	#if 1	//xinsy20140326
		LOG_INFO << "Average_WindSpeed_10min : " << sAverage_WindSpeed_10min;
		LOG_INFO << "Average_WindDirection_10min : " << sAverage_WindDirection_10min;
		LOG_INFO << "Max_WindSpeed : " << sMax_WindSpeed;
		LOG_INFO << "Extreme_WindSpeed : " << sExtreme_WindSpeed;
		LOG_INFO << "Standard_WindSpeed : " << sStandard_WindSpeed;
		LOG_INFO << "Air_Temperature : " << sAir_Temperature;
		LOG_INFO << "Humidity : " << sHumidity;
		LOG_INFO << "Air_Pressure : " << sAir_Pressure;
		LOG_INFO << "Precipitation : " << sPrecipitation;
		LOG_INFO << "Precipitation_Intensity : " << sPrecipitation_Intensity;
		LOG_INFO << "Radiation_Intensity : " << sRadiation_Intensity;
		LOG_INFO << "Precipitation : " << sPrecipitation;
	#endif 

		ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
		ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
		ret.insert(make_pair<string,string>("Average_WindSpeed_10min",sAverage_WindSpeed_10min));
		ret.insert(make_pair<string,string>("Average_WindDirection_10min",sAverage_WindDirection_10min));
		ret.insert(make_pair<string,string>("Max_WindSpeed",sMax_WindSpeed));
		ret.insert(make_pair<string,string>("Extreme_WindSpeed",sExtreme_WindSpeed));
		ret.insert(make_pair<string,string>("Standard_WindSpeed",sStandard_WindSpeed));
		ret.insert(make_pair<string,string>("Air_Temperature",sAir_Temperature));
		ret.insert(make_pair<string,string>("Humidity",sHumidity));
		ret.insert(make_pair<string,string>("Air_Pressure",sAir_Pressure));
		ret.insert(make_pair<string,string>("Precipitation",sPrecipitation));
		ret.insert(make_pair<string,string>("Precipitation_Intensity",sPrecipitation_Intensity));
		ret.insert(make_pair<string,string>("Radiation_Intensity",sRadiation_Intensity));

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
