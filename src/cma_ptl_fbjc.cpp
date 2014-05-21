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
namespace cma_fbjc_parser
{
string sqlInsertStringMaker(const cma_frame& frm)
{
    map<string,string> mp = frm.getMoniDataMap();
    string ret("insert into sd_fbjc (CDID, AcquisitionTime, Equal_IceThickness, Tension, Tension_Difference, Windage_Yaw_Angle,Deflection_Angle) VALUES");

    ret += "(";
    
    ret += "'";
    ret += frm.getCdid();
    ret += "',"; 

    map<string,string>::iterator itr;
    
    itr = mp.find("Time_Stamp");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";


    itr = mp.find("Equal_IceThickness");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Tension");
    ret += "'";
    ret += (itr == mp.end()) ? "" : itr->second;
    ret += "',";

    itr = mp.find("Tension_Difference");
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

    uint32_t timestamp = makelong(makeword(data[17],data[18]),makeword(data[19],data[20]));
    uint16_t alertflag = makeword(data[21],data[22]);
    uint32_t equalicethickness = makelong(makeword(data[23],data[24]),makeword(data[25],data[26]));
    uint32_t tension = makelong(makeword(data[27],data[28]),makeword(data[29],data[30]));
    uint32_t tensiondifference = makelong(makeword(data[31],data[32]),makeword(data[33],data[34]));
    uint32_t windageyawangle = makelong(makeword(data[35],data[36]),makeword(data[37],data[38]));
    uint32_t deflectionangle = makelong(makeword(data[39],data[40]),makeword(data[41],data[42]));
    uint32_t reverse1 = makelong(makeword(data[43],data[44]),makeword(data[45],data[46]));
    uint32_t reverse2 = makelong(makeword(data[47],data[48]),makeword(data[49],data[50]));

    try
    {
    //    string sTimestamp = boost::lexical_cast<string>(timestamp);
        uint64_t ms = timestamp;
        ms *= 1000;
        ms *= 1000;
        Timestamp ts(ms);
        string sTimestamp = ts.toFormattedStringDash(); 
        string sAlertflag = boost::lexical_cast<string>(alertflag);
        string sEqualicethickness = boost::lexical_cast<string>(equalicethickness);
        string sTension = boost::lexical_cast<string>(tension);
        string sTensiondifference = boost::lexical_cast<string>(tensiondifference);
        string sWindageyawangle = boost::lexical_cast<string>(windageyawangle);
        string sDeflectionangle = boost::lexical_cast<string>(deflectionangle);
        string sReverse1 = boost::lexical_cast<string>(reverse1);
        string sReverse2 = boost::lexical_cast<string>(reverse2);

        ret.insert(make_pair<string,string>("Time_Stamp",sTimestamp));
        ret.insert(make_pair<string,string>("Alert_Flag",sAlertflag));
        ret.insert(make_pair<string,string>("Equal_IceThickness",sEqualicethickness));
        ret.insert(make_pair<string,string>("Tension",sTension));
        ret.insert(make_pair<string,string>("Tension_Difference",sTensiondifference));
        ret.insert(make_pair<string,string>("Windage_Yaw_Angle",sWindageyawangle));
        ret.insert(make_pair<string,string>("Deflection_Angle",sDeflectionangle));
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
