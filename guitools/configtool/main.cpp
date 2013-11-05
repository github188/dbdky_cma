#include <ncurses.h>
#include <form.h>

#include <tinyxml.h>
#include "ConfigUtil.h"

#include <map>
#include <string>
#include <utility>

#include <boost/lexical_cast.hpp>

using namespace std;
using dbdky::port::ConfUtil;

map<string,string> configs;

FIELD **fields = NULL;

void configs2Fields()
{
    if (*fields)
    {
        delete [] *fields;
    }

    *fields = new FIELD[configs.size() + 1];

}

void updateConfigs()
{
   configs.clear();
   
   string sProxyip = ConfUtil::getInstance()->getProxyIP();
   string sProxyPort = boost::lexical_cast<string>(ConfUtil::getInstance()->getProxyPort()); 
   string sSystemTick = boost::lexical_cast<string>(ConfUtil::getInstance()->getSystemTick());
   string sHeartbeatTick = boost::lexical_cast<string>(ConfUtil::getInstance()->getHeartbeatTick());
   string sUploadMoniDataTick = boost::lexical_cast<string>(ConfUtil::getInstance()->getUploadMoniDataTick());
   string sDBPath = ConfUtil::getInstance()->getDBPath();
   string sDBUser = ConfUtil::getInstance()->getDBUser();
   string sDBPasswd = ConfUtil::getInstance()->getDBPasswd();
   string sDBName = ConfUtil::getInstance()->getDBName();
   string sCmaId = ConfUtil::getInstance()->getCmaId();
   
   configs.insert(make_pair<string,string>("proxy_ip", sProxyip));
   configs.insert(make_pair<string,string>("proxy_port", sProxyPort));
   configs.insert(make_pair<string,string>("system_tick", sSystemTick));
   configs.insert(make_pair<string,string>("heartbeat_tick", sHeartbeatTick));
   configs.insert(make_pair<string,string>("uploadmonidata_tick", sUploadMoniDataTick));
   configs.insert(make_pair<string,string>("db_path", sDBPath));
   configs.insert(make_pair<string,string>("db_user", sDBUser));
   configs.insert(make_pair<string,string>("db_passwd", sDBPasswd));
   configs.insert(make_pair<string,string>("db_name", sDBName));
   configs.insert(make_pair<string,string>("cmaid", sCmaId));

   configs2Fields();
}

int main()
{

    return 0;
}
