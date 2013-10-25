#ifndef __CMA_SERVER_H_
#define __CMA_SERVER_H_

#include <port/TcpServer.h>
#include <port/EventLoop.h>
#include <boost/noncopyable.hpp>

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

    const string name_;
};
}
}

#endif
