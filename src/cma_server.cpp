#include "cma_server.h"

#include <utils/Logging.h>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include "tinyxml.h"

namespace dbdky
{
namespace cma_server
{
cma_server::cma_server(dbdky::port::EventLoop* loop,
            const dbdky::port::InetAddress& listenAddr,
            const string& nameArg)
    : loop_(loop),
      server_(loop, listenAddr, "cma_server"),
      name_(nameArg)
{
    server_.setConnectionCallback(
        boost::bind(&cma_server::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&cma_server::onMessage, this, _1, _2, _3));
}

void cma_server::start()
{
    server_.start();
}

void cma_server::onConnection(const dbdky::port::TcpConnectionPtr& conn)
{
    LOG_INFO << "cma_server - " << conn->peerAddress().toIpPort() << " ->  "
	<< conn->localAddress().toIpPort() << " is "
 	<< (conn->connected() ? "UP" : "DOWN");
}

void cma_server::onMessage(const dbdky::port::TcpConnectionPtr& conn,
		dbdky::port::Buffer* buf,
		dbdky::Timestamp time)
{
    dbdky::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << "receive at [" << time.toString()
		<< "]" << msg.size() << "'bytes: " << msg;

    //TODO:
}

}
}
