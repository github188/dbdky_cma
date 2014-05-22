#ifndef __CMA_SERVER_LEGACY_H_
#define __CMA_SERVER_LEGACY_H_

#include <port/UdpServer.h>
#include <port/EventLoop.h>

#include <port/Buffer.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <utils/Timestamp.h>
#include <utils/ThreadPool.h>

#include <dbhelper/dbhelper.h>

#include "cma_frame_legacy.h"

namespace dbdky
{
namespace cma_server
{
class cma_server_legacy : boost::noncopyable
{
public:
    cma_server_legacy(dbdky::port::EventLoop* loop,
        const dbdky::port::InetAddress& listenAddr,
        const string& nameArg);

    void start();

private:
    uint16_t makeResponseFrame(bool ok,const cma_frame_legacy &frame,const uint8_t **serialData);	//xinsy20140327
    void onMessage(dbdky::port::Buffer* buf,
        dbdky::Timestamp time);

    dbdky::port::EventLoop* loop_;
    dbdky::port::UdpServer server_;

    const string name_;

    boost::scoped_ptr<DBHelper> dbhelper_;

    mutable MutexLock mutexTimestmap_;
    boost::scoped_ptr<dbdky::port::EventLoopThreadPool> threadPool_;
    uint64_t optimestamp_;

    string getCdidOfFrame(const cma_frame_legacy& frm);
    map<string,string> cdiddirec_;

};
}
}

#endif    //__CMA_SERVER_LEGACY_H_
