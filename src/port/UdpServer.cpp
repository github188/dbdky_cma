#include <port/UdpServer.h>

#include <utils/Logging.h>

#include <boost/bind.hpp>

using namespace dbdky;
using namespace dbdky::port;

UdpServer::UdpServer(EventLoop* loop,
	const InetAddress& listenAddr,
	const string& nameArg)
	: loop_(CHECK_NOTNULL(loop)),
	  hostport_(listenAddr.toIpPort()),
	  name_(nameArg),
	  messageCallback_(defaultUdpMessageCallback),
	  started_(false)
{
}

UdpServer::~UdpServer()
{
    //TODO:
}

void UdpServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void UdpServer::start()
{
    if (!started_)
    {
        started_ = true;
        threadPool_->start(threadInitCallback_);
    }

    //TODO:
    // start listening...
}


