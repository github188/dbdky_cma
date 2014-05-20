#include "cma_uploadservice.h"
#include "ConfigUtil.h"
#include "Default_defs.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>
#include <utils/Timestamp.h>

#include "ws/test/soapCAGAccessServiceHttpBindingProxy.h"
#include "ws/test/CAGAccessServiceHttpBinding.nsmap"

namespace dbdky
{
namespace detail
{
string upLoadCMAData(const string& msg)
{
    CAGAccessServiceHttpBindingProxy service;
    _ns1__uploadCMAData param;
    _ns1__uploadCMADataResponse response;

    param.in0 = new std::string(msg);
    
    if (service.uploadCMAData(&param, &response) == SOAP_OK)
    {
        if (NULL == response.out)
        {
            return string("");
        }
   
        return *(response.out);
    }
    else
    {
        service.soap_stream_fault(std::cerr);
        return string("");
    }

    service.destroy();
}
}
}

namespace dbdky
{
namespace cma_server
{
    cma_uploadservice::cma_uploadservice(EventLoop* loop,
                const string& name)
        : loop_(loop),
          name_(name),
          updateIntervalCounter_(0),
          threadPool_(new EventLoopThreadPool(loop)),
          dbhelper_(new DBHelper(ConfUtil::getInstance()->getDBPath(),
                                ConfUtil::getInstance()->getDBUser(),
                                ConfUtil::getInstance()->getDBPasswd(),
                                ConfUtil::getInstance()->getDBName())),
          updateTimestamp_(ConfUtil::getInstance()->getLastUploadTime())
    {
        threadPool_->setThreadNum(6);
        threadPool_->start();
    }

    cma_uploadservice::~cma_uploadservice()
    {
    }

    void cma_uploadservice::start()
    {
        systemTimer_ = loop_->runEvery(ConfUtil::getInstance()->getSystemTick(),
            boost::bind(&cma_uploadservice::onSystemTimer, this));

        heartBeatTimer_ = loop_->runEvery(ConfUtil::getInstance()->getHeartbeatTick(),
            boost::bind(&cma_uploadservice::onHeartbeatTimer, this));

        uploadMoniDataTimer_ = loop_->runEvery(ConfUtil::getInstance()->getUploadMoniDataTick(),
            boost::bind(&cma_uploadservice::onUploadMoniDataTimer, this));
    }

    void cma_uploadservice::stop()
    {
    }

    void cma_uploadservice::onSystemTimer()
    {
        uint64_t tmpTs = Timestamp::now().microSecondsSinceEpoch();

        {
            MutexLockGuard lock(mutexTimestamp_);
            updateTimestamp_ = tmpTs;
        }

        string sTs;

        try
        {
            sTs = boost::lexical_cast<string>(tmpTs);
        }
        catch (boost::bad_lexical_cast& e)
        {
            LOG_ERROR << "Can't cast timestmap";
            return;
        }

        if (++updateIntervalCounter_ >= DEFAULT_UPDATE_INTERVAL_COUNTER)
        {
            boost::bind(&ConfUtil::saveItem2File, ConfUtil::getInstance(), _1, _2)("last_upload", sTs);
            updateIntervalCounter_ = 0;
        }
    }

    void cma_uploadservice::onHeartbeatTimer()
    {
        loop_->queueInLoop(
            boost::bind(&cma_uploadservice::uploadHeartbeatTask, this));
    }

    void cma_uploadservice::onUploadMoniDataTimer()
    {
        loop_->queueInLoop(
            boost::bind(&cma_uploadservice::uploadMoniDataTask, this));
    }

    void cma_uploadservice::uploadHeartbeatTask()
    {
        string sJczzid;

        if (!dbhelper_->isConnected())
        {
            if (!dbhelper_->connect())
            {
                LOG_ERROR << "Connect database fail.";
                return;
            }
        }

        boost::shared_ptr<ResultSet> result(dbhelper_->query("select OBJID ,LinkedDevice,DeviceCode,ZIEDID,IEDID,Phase,JCLXBM from sd_cd"));
        if (!result.get())
        {
            LOG_INFO << "*****";
            return;
        }

        string sTmp("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        sTmp += "<request><cma id=\"";
        sTmp += ConfUtil::getInstance()->getCmaId();
        sTmp += "\">";
        sTmp += "<ip>";
        sTmp += ConfUtil::getInstance()->getLocalIP();
        sTmp += "</ip>";
        sTmp += "<curtime>";
        sTmp += Timestamp::now().toFormattedStringDash();
        sTmp += "</curtime>";
        sTmp += "<operationtemperature>15.00</operationtemperature>";
        sTmp += "<sensors>";

        string sTmp1;
        try
        {
            while (result->next())
            {
                sJczzid = result->getString("DeviceCode");
                sTmp1 += "<sensor id = \"";
                sTmp1 += sJczzid;
                sTmp1 += "\">";

                sTmp1 += "<status>NORMAL</status>";
                sTmp1 += "<operationtemperature>15.00</operationtemperature>";
                sTmp1 += "</sensor>"; 
            } 
        }
        catch (std::exception& e)
        {
            LOG_ERROR << e.what();
        }

        sTmp += sTmp1;
        sTmp += "</sensors></request>";
      
        LOG_INFO << sTmp; 
        //TODO:
        //upload it 
    }

    void cma_uploadservice::uploadMoniDataTask()
    {
        string sql = "select OBJID,LinkedDevice,DeviceCode,ZIEDID,IEDID,Phase,JCLXBM from sd_cd";
        int iCount = 0;

        if (!dbhelper_->isConnected())
        {
            if (!dbhelper_->connect())
            {
                LOG_ERROR << "Connect database fail.";
                return;
            }
        }

        boost::shared_ptr<ResultSet> result(dbhelper_->query(sql));
        if (!result.get())
        {
            LOG_INFO << "Cannot get data from database";
            return;
        }

        string sTmp("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        
        try
        {
            sTmp += "<request><monitordata cmaid=\"";
            sTmp += ConfUtil::getInstance()->getCmaId();
            sTmp += "\" nodecount=\"init_rowcount\">";
    
            while (result->next())
            {
                string sObjid = result->getString("OBJID");
                string sDeviceCode = result->getString("DeviceCode");
                string sLinkedDevice = result->getString("LinkedDevice");
                string sZIEDID = result->getString("ZIEDID");
                string sIEDID = result->getString("IEDID");
                string sJCLXBM = result->getString("JCLXBM");
                string sPhase = result->getString("Phase"); 

                string sCdType = getCdType(sJCLXBM); 

                string tmpJCLXBM(sJCLXBM);
                std::transform(tmpJCLXBM.begin(), tmpJCLXBM.end(), tmpJCLXBM.begin(), ::tolower);
                uint64_t tmpTs;
                {
                    MutexLockGuard lock(mutexTimestamp_);
                    tmpTs = updateTimestamp_;
                }
 
//                string sQuery = "select * from " + sJCLXBM
                string sQuery = "select * from " + tmpJCLXBM
 			+ " where cdid = " + sObjid
			+ " and AcquisitionTime > " + "'"
			+ Timestamp(tmpTs).toFormattedStringDash() + "';";

                if (!dbhelper_->isConnected())
                {
                    dbhelper_->connect();
                }

                boost::shared_ptr<ResultSet> tmpresult =
			dbhelper_->query(sQuery.c_str());

                if (!tmpresult.get())
                {
                    LOG_INFO << "*** Query return empty";
                    return;
                }
                else
                {
                    while (tmpresult->next())
                    {
                        string timestamp = tmpresult->getString("AcquisitionTime");
                        sTmp += "<datenode sensorid=\"" + sDeviceCode + "\"> ";
			sTmp += "<type>" + sCdType + "</type>";
                        sTmp += "<equipmentid>" + sLinkedDevice + "</equipmentid>";
			sTmp += "<timestamp>" + timestamp + "</timestamp>";
			sTmp += "<attrs>";
			
                        if ("SD_GTQX" == sJCLXBM)
        		{
                            string Inclination = tmpresult->getString("Inclination");
                            string InclinationX = tmpresult->getString("INCLINATION_X");
                            string InclinationY = tmpresult->getString("Inclination_Y");
                            string sGtAngleX = tmpresult->getString("Angle_X");
                            string sGtAngleY = tmpresult->getString("Angle_Y");

                            sTmp += "<attr name=\"INCLINATION\" value=\"" + Inclination + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"INCLINATION_X\" value=\"" + InclinationX + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"INCLINATION_Y\" value=\"" + InclinationY + "\" alarm=\"False\"/>";
 			    sTmp += "<attr name=\"Angle_X\" value=\"" + sGtAngleX + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Angle_Y\" value=\"" + sGtAngleY + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_DXHC" == sJCLXBM)
        		{
                            string Conductor_Sag = tmpresult->getString("CONDUCTOR_SAG");
                            string Toground_Distance = tmpresult->getString("Toground_Distance");
                            string Angle = tmpresult->getString("Angle");
                            string Measure_Flag = tmpresult->getString("Measure_Flag");

                            sTmp += "<attr name=\"Conductor_Sag\" value=\"" + Conductor_Sag + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Toground_Distance\" value=\"" + Toground_Distance + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Angle\" value=\"" + Angle + "\"/>";
                            sTmp += "<attr name=\"Measure_Flag\" value=\"" + Measure_Flag + "\"/>";
        		}
        		else if ("SD_DXWD" == sJCLXBM)
        		{
                            string LINE_TEMPERATURE1 = tmpresult->getString("LINE_TEMPERATURE1");
                            string LINE_TEMPERATURE2 = tmpresult->getString("LINE_TEMPERATURE2");
        
                            sTmp += "<attr name=\"LINE_TEMPERATURE1\" value=\"" + LINE_TEMPERATURE1 + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"LINE_TEMPERATURE2\" value=\"" + LINE_TEMPERATURE2 + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_WFZD" == sJCLXBM)
        		{
  			    string VIBRATION_AMPLITUDE = tmpresult->getString("VIBRATION_AMPLITUDE");
                            string VIBRATION_FREQUENCY = tmpresult->getString("VIBRATION_FREQUENCY");

                            sTmp += "<attr name=\"VIBRATION_AMPLITUDE\" value=\"" + VIBRATION_AMPLITUDE + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"VIBRATION_FREQUENCY\" value=\"" + VIBRATION_FREQUENCY + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_XLFP" == sJCLXBM)
        		{
                            string Windage_Yaw_Angle = tmpresult->getString("Windage_Yaw_Angle");
                            string Deflection_Angle = tmpresult->getString("Deflection_Angle");
                            string Least_Clearance = tmpresult->getString("Least_Clearance");

                            sTmp += "<attr name=\"Windage_Yaw_Angle\" value=\"" + Windage_Yaw_Angle + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Deflection_Angle\" value=\"" + Deflection_Angle + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Least_Clearance\" value=\"" + Least_Clearance + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_FBJC" == sJCLXBM)
        		{
                            string Equal_IceThickness = tmpresult->getString("Equal_IceThickness");
                            string Tension = tmpresult->getString("Tension");
                            string Tension_Difference = tmpresult->getString("Tension_Difference");
                            string Windage_Yaw_Angle = tmpresult->getString("Windage_Yaw_Angle");
                            string Deflection_Angle = tmpresult->getString("Deflection_Angle");
 
                            sTmp += "<attr name=\"Equal_IceThickness\" value=\"" + Equal_IceThickness + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Tension\" value=\"" + Tension + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Tension_Difference\" value=\"" + Tension_Difference + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Windage_Yaw_Angle\" value=\"" + Windage_Yaw_Angle + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Deflection_Angle\" value=\"" + Deflection_Angle + "\" alarm=\"False\"/>";
        		}
       			 else if ("SD_WDJC" == sJCLXBM)
        		{
                            string U_Gallop_Amplitude = tmpresult->getString("U_Gallop_Amplitude");
                            string U_Vertical_Amplitude = tmpresult->getString("U_Vertical_Amplitude");
                            string U_Horizontal_Amplitude = tmpresult->getString("U_Horizontal_Amplitude");
                            string U_AngleToVertical = tmpresult->getString("U_AngleToVertical");
                            string U_Gallop_Frequency = tmpresult->getString("U_Gallop_Frequency");

                            sTmp += "<attr name=\"U_Gallop_Amplitude\" value=\"" + U_Gallop_Amplitude + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"U_Vertical_Amplitude\" value=\"" + U_Vertical_Amplitude + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"U_Horizontal_Amplitude\" value=\"" + U_Horizontal_Amplitude + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"U_AngleToVertical\" value=\"" + U_AngleToVertical + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"U_Gallop_Frequency\" value=\"" + U_Gallop_Frequency + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_JYZWH" == sJCLXBM)
        		{
                            string ESDD = tmpresult->getString("ESDD");
                            string NSDD = tmpresult->getString("NSDD");
                            string Daily_Max_Temperature = tmpresult->getString("Daily_Max_Temperature");
                            string Daily_Min_Temperature = tmpresult->getString("Daily_Min_Temperature");
                            string Daily_Max_Humidity = tmpresult->getString("Daily_Max_Humidity");
                            string Daily_Min_Humidity = tmpresult->getString("Daily_Min_Humidity");

                            sTmp += "<attr name=\"ESDD\" value=\"" + ESDD + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"NSDD\" value=\"" + NSDD + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Daily_Max_Temperature\" value=\"" + Daily_Max_Temperature + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Daily_Min_Temperature\" value=\"" + Daily_Min_Temperature + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Daily_Max_Humidity\" value=\"" + Daily_Max_Humidity + "\" alarm=\"False\"/>";
                            sTmp += "<attr name=\"Daily_Min_Humidity\" value=\"" + Daily_Min_Humidity + "\" alarm=\"False\"/>";
        		}
        		else if ("SD_QX" == sJCLXBM)
        		{
                            string Average_WindSpeed_10min = tmpresult->getString("Average_WindSpeed_10min");
                            string Average_WindDirection_10min = tmpresult->getString("Average_WindDirection_10min");
                            string Max_WindSpeed = tmpresult->getString("Max_WindSpeed");
                            string Extreme_WindSpeed = tmpresult->getString("Extreme_WindSpeed");
                            string Standard_WindSpeed = tmpresult->getString("Standard_WindSpeed");
                            string Air_Temperature = tmpresult->getString("Air_Temperature");
                            string Humidity = tmpresult->getString("Humidity");
                            
                            string Air_Pressure = tmpresult->getString("Air_Pressure");
                            string Precipitation = tmpresult->getString("Precipitation");
                            string Precipitation_Intensity = tmpresult->getString("Precipitation_Intensity");
                            string Radiation_Intensity = tmpresult->getString("Radiation_Intensity");

                            sTmp += "<attr name=\"Average_WindSpeed_10min\" value=\"" + Average_WindSpeed_10min + "\"/>";
                            sTmp += "<attr name=\"Average_WindDirection_10min\" value=\"" + Average_WindDirection_10min + "\"/>";
                            sTmp += "<attr name=\"Max_WindSpeed\" value=\"" + Max_WindSpeed + "\"/>";
                            sTmp += "<attr name=\"Extreme_WindSpeed\" value=\"" + Extreme_WindSpeed + "\"/>";
                            sTmp += "<attr name=\"Standard_WindSpeed\" value=\"" + Standard_WindSpeed + "\"/>";
                            sTmp += "<attr name=\"Air_Temperature\" value=\"" + Air_Temperature + "\"/>";
                            sTmp += "<attr name=\"Humidity\" value=\"" + Humidity + "\"/>";
                            sTmp += "<attr name=\"Air_Pressure\" value=\"" + Air_Pressure + "\"/>";
                            sTmp += "<attr name=\"Precipitation\" value=\"" + Precipitation + "\"/>";
                            sTmp += "<attr name=\"Precipitation_Intensity\" value=\"" + Precipitation_Intensity + "\"/>";
                            sTmp += "<attr name=\"Radiation_Intensity\" value=\"" + Radiation_Intensity + "\"/>";
        		}
        		else if ("SD_TXJC" == sJCLXBM)
        		{
                            string CHANNELNO = tmpresult->getString("CHANNELNO");
                            string PRESETPOSITION = tmpresult->getString("PRESETPOSITION");
                            string ISTYPICAL = tmpresult->getString("ISTYPICAL");
                            string CATEGORY = tmpresult->getString("CATEGORY");
                            string IMAGEORVIDEO = tmpresult->getString("IMAGEORVIDEO");
                            string RESOLUTION = tmpresult->getString("RESOLUTION");

                            sTmp += "<attr name=\"CHANNELNO\" value=\"" + CHANNELNO + "\"/>";
                            sTmp += "<attr name=\"PRESETPOSITION\" value=\"" + PRESETPOSITION + "\"/>";
                            sTmp += "<attr name=\"ISTYPICAL\" value=\"" + ISTYPICAL + "\"/>";
                            sTmp += "<attr name=\"CATEGORY\" value=\"" + CATEGORY + "\"/>";
                            sTmp += "<attr name=\"RESOLUTION\" value=\"" + RESOLUTION + "\"/>";
        		}
  
                        sTmp += "</attrs></datanode>";
                    }
                    
		    tmpresult->last();
                    iCount += tmpresult->getRow();
                }
            }
        }
        catch (std::exception& e)
        {
            LOG_ERROR << e.what();
            return;
        }
       
        sTmp += "</monitordata></request>";

        string sICount;
        try
        {
            sICount = boost::lexical_cast<string>(iCount);
        }
        catch (boost::bad_lexical_cast& e)
        {
            sICount += "0";
        }
 
        boost::replace_first(sTmp, "init_rowcount", sICount);

        //TODO:
        LOG_INFO << sTmp;
    }

    string cma_uploadservice::getCdType(string str)
    {
        string sCdType;
        if ("SD_GTQX" == str)
        {
            sCdType += "012001";
        }
        else if ("SD_DXHC" == str)
        {
            sCdType += "013001";
        }
        else if ("SD_DXWD" == str)
        {
            sCdType += "013002";
        }
        else if ("SD_WFZD" == str)
        {
            sCdType += "013003";
        }
        else if ("SD_XLFP" == str)
        {
            sCdType += "013004";
        }
        else if ("SD_FBJC" == str)
        {
            sCdType += "013005";
        }
        else if ("SD_WDJC" == str)
        {
            sCdType += "013006";
        }
        else if ("SD_JYZWH" == str)
        {
            sCdType += "014001";
        }
        else if ("SD_QX" == str)
        {
            sCdType += "018001";
        }
        else if ("SD_TXJC" == str)
        {
            sCdType += "018002";
        }

        return sCdType;
    }
}
}
