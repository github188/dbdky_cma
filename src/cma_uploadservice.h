#ifndef __CMA_UPLOADSERVICE_H_
#define __CMA_UPLOADSERVICE_H_

#include <port/EventLoop.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <port/Callbacks.h>
#include <port/Buffer.h>
#include <utils/Timestamp.h>
#include <port/TcpClient.h>
#include <port/TcpConnection.h>

#include <port/EventLoop.h>
#include <port/EventLoopThreadPool.h>

#include <dbhelper/dbhelper.h>

#include <string>

using namespace dbdky;
using namespace dbdky::port;

using std::string;

namespace dbdky
{
namespace cma_server
{
class cma_uploadservice : boost::noncopyable
{
public:
    typedef boost::shared_ptr<ResultSet> ResultSetPtr;
    typedef boost::function <string (const ResultSetPtr&)> NodeXmlMaker;

    void start();
    void stop();
    explicit cma_uploadservice(EventLoop* loop,
                    const string& name);

    ~cma_uploadservice();


private:
    EventLoop* loop_;
    TimerId systemTimer_;
    TimerId heartBeatTimer_;
    TimerId uploadMoniDataTimer_;

    const string name_;
    const string cmaid_;
 
    uint16_t updateIntervalCounter_;
    
    mutable MutexLock mutex_;
    mutable MutexLock mutexTimestamp_;
  
    uint64_t updateTimestamp_;

    
    boost::scoped_ptr<EventLoopThreadPool> threadPool_;
    boost::scoped_ptr<DBHelper> dbhelper_;

    void onSystemTimer();
    void onHeartbeatTimer();
    void onUploadMoniDataTimer();

    void uploadMoniDataTask();
    void uploadHeartbeatTask();

    string getCdType(string);
}; 
}
}


#endif
