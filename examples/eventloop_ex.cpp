#include <utils/Logging.h>
#include <port/EventLoop.h>
#include <utils/Timestamp.h>
#include <port/Channel.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

using namespace dbdky;
using namespace dbdky::port;

void defaultWriteEventCallback()
{
    LOG_INFO << "defaultWriteEventCallback";
}

void defaultCloseEventCallback()
{
    LOG_INFO << "defaultCloseEventCallback";
}

void defaultReadEventCallback(Timestamp ts)
{
    LOG_INFO << "defaultReadEventCallback [" << ts.toFormattedString() << "]";
}

int main(int argc, char** argv)
{
    EventLoop loop;
    int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,6);
    if (fd < 0)
    {
        LOG_INFO << "Fail to open test file";
        return -1;
    }
    dbdky::port::Channel chl(&loop, fd);
    chl.setWriteCallback(boost::bind(&defaultWriteEventCallback));
    chl.setCloseCallback(boost::bind(&defaultCloseEventCallback));
    //chl.setReadCallback(boost::bind(&defaultReadEventCallback)(_1));
   
    loop.updateChannel(&chl);

    loop.loop();
    
    return 0;    
}
