#ifndef __PORT_UDPLISTENER_H_
#define __PORT_UDPLISTENER_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <port/Socket.h>

namespace dbdky
{
namespace port
{
class EventLoop;
class InetAddress;

class UdpListener : boost::noncopyable
{
public:
    UdpListener(EventLoop* loop, const InetAddress& listenAddr);

    ~UdpListener();

    bool listenning() const
    {
        return listenning_;
    }

    void listen();

private:

    EventLoop* loop_;
    Socket listenSocket_;

    bool listenning_;
};
}
}


#endif    //__PORT_UDPLISTENER_H_
