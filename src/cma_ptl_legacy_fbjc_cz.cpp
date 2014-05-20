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
#include <Hex2FloatConvert.h>

#include <dbhelper/dbhelper.h>
#include "ConfigUtil.h"

using namespace std;
using dbdky::cma_server::DBHelper;
using dbdky::cma_server::cma_frame;
using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_legacy_fbjc_cz_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
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

map<string,string> frameParserFunc(cma_frame& frm)
{
	map<string,string>ret;
    //TODO:

    return ret;	
}
}
}
