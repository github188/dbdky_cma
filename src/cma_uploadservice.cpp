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
            tmpTs = updateTimestamp_;
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
            boost::bind(&ConfUtil::saveItem2File, ConfUtil::getInstance(), _1, _2)("last_update", sTs);
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
        //TODO:
    }

    void cma_uploadservice::uploadMoniDataTask()
    {
        //TODO:
    }
}
}
