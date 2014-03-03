#include <port/UdpListener.h>

#include <port/EventLoop.h>
#include <port/InetAddress.h>
#include <port/SocketsOps.h>

#include <boost/bind.hpp>

#include <errno.h>
#include <fcntl.h>

using namespace dbdky;
using namespace dbdky::port;

UdpListener::UdpListener(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop),
      listenSocket_(sockets::createNonblockingOrDie()),
      listenning_(false)
{
    listenSocket_.bindAddress(listenAddr);
}

UdpListener::~UdpListener()
{
}

void UdpListener::listen()
{
    loop_->assertInLoopThread();
    listenning_ = true;
    listenSocket_.listen();
}

