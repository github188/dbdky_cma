#include "cma_server.h"

#include <utils/Logging.h>
#include <port/EventLoop.h>

#include "ConfigUtil.h"

using namespace dbdky;
using namespace dbdky::port;
using namespace dbdky::cma_server;

int main(int argc, char** argv)
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
//    InetAddress listenAddr("127.0.0.1", 6001);
    InetAddress listenAddr(ConfUtil::getInstance()->getProxyIP(),
			ConfUtil::getInstance()->getProxyPort());

    dbdky::cma_server::cma_server server(&loop, listenAddr, "cma_server");
    server.start();
    loop.loop();

    return 0;
}
