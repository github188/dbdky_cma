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
#include <Hex2FloatConvert.h>

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame_legacy;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_legacy_fbjc_cz_parser
{
string sqlInsertStringMaker(const cma_frame_legacy& frm)
{
	map<string,string> mp =  frm.getMoniDataMap();
	string ret("insert into sd_fbjc (CDID, AcquisitionTime, Tension, Windage_Yaw_Angle, Deflection_Angle, Battery_Voltage, Operation_Temperature) VALUES");

	ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',";

    map<string,string>::iterator itr;

    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',",

    itr = mp.find("Tension");
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
	map<string,string>ret;
    char sensorId[2];
 
    const uint8_t* data = const_cast<cma_frame_legacy*>(&frm)->getPduData();
    memcpy(sensorId, data, 2);

    uint32_t timestamp = makelong(makeword(data[2], data[3]), 
        makeword(data[4], data[5]));

    uint16_t tension = makeword(data[6], data[7]);
    uint16_t windage_yaw_angle = makeword(data[8], data[9]);
    uint16_t deflection_angle = makeword(data[10], data[11]);
    uint16_t battery_voltage = makeword(data[12], data[13]);
    uint16_t operation_temperature = makeword(data[14], data[15]);
    uint16_t reserve1 = makeword(data[16], data[17]);
    uint16_t reserve2 = makeword(data[18], data[19]);

    try
    {
        uint64_t ms = timestamp;
        ms *= 1000;
        ms *= 1000;

        Timestamp ts(ms);

        string sTimestamp = ts.toFormattedStringDash();
        string sTension = boost::lexical_cast<string>(sTension);
        string sWindage_yaw_angle = boost::lexical_cast<string>(windage_yaw_angle);
        string sDeflection_angle = boost::lexical_cast<string>(deflection_angle);
        string sBattery_voltage = boost::lexical_cast<string>(battery_voltage);
        string sOperation_temperature = boost::lexical_cast<string>(operation_temperature);
        string sReserve1 = boost::lexical_cast<string>(reserve1);
        string sReserve2 = boost::lexical_cast<string>(reserve2);

        ret.insert(make_pair<string,string>("Time_Stemp", sTimestamp));
        ret.insert(make_pair<string,string>("Tension", sTension));
        ret.insert(make_pair<string,string>("Windage_Yaw_Angle", sWindage_yaw_angle));
        ret.insert(make_pair<string,string>("Deflection_Angle", sDeflection_angle));
        ret.insert(make_pair<string,string>("Battery_Voltage", sBattery_voltage));
        ret.insert(make_pair<string,string>("Operation_Temperature", sOperation_temperature));
        ret.insert(make_pair<string,string>("Reserve1", sReserve1));
        ret.insert(make_pair<string,string>("Reserve2", sReserve2));
    }
    catch (boost::bad_lexical_cast& e)
    {
        return ret;
    }


    return ret;	
}
}
}
