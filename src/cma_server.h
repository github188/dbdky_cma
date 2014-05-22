#ifndef __CMA_SERVER_H_
#define __CMA_SERVER_H_

#include <port/TcpServer.h>
#include <port/EventLoop.h>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <utils/Timestamp.h>

#include <port/EventLoopThreadPool.h>
#include <dbhelper/dbhelper.h>

#include "cma_frame.h"

namespace dbdky
{
namespace cma_server
{
class cma_server : boost::noncopyable
{
public:
    cma_server(dbdky::port::EventLoop* loop,
        const dbdky::port::InetAddress& listenAddr,
        const string& nameArg);

    void start();

private:
    void onConnection(const dbdky::port::TcpConnectionPtr& conn);
    void onMessage(const dbdky::port::TcpConnectionPtr& conn,
        dbdky::port::Buffer* buf,
        dbdky::Timestamp time);
    uint16_t makeResponseFrame(bool ok,const cma_frame &frame,const uint8_t **serialData);	//xinsy20140324

    dbdky::port::EventLoop* loop_;
    dbdky::port::TcpServer server_;

    boost::scoped_ptr<DBHelper> dbhelper_;

    mutable MutexLock mutexTimestamp_;
    boost::scoped_ptr<dbdky::port::EventLoopThreadPool> threadPool_;
    uint64_t optimestamp_;

    const string name_;
    string getCdidOfFrame(const cma_frame& frm);
    map<string,string> cdiddirec_;
};
}
}

#endif
