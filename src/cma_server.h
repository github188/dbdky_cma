#ifndef __CMA_SERVER_H_
#define __CMA_SERVER_H_

#include <port/TcpServer.h>
#include <port/EventLoop.h>
#include <boost/noncopyable.hpp>


#include <utils/Timestamp.h>

#include <port/EventLoopThreadPool.h>


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

    dbdky::port::EventLoop* loop_;
    dbdky::port::TcpServer server_;

    mutable MutexLock mutexTimestamp_;
    boost::scoped_ptr<dbdky::port::EventLoopThreadPool> threadPool_;
    uint64_t optimestamp_;

    const string name_;
};
}
}

#endif
