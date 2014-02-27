#include "cma_server_legacy.h"

#include <utils/Logging.h>
#include <port/EventLoop.h>

using namespace dbdky;
using namespace dbdky::port;
using namespace dbdky::cma_server;

int main(int argc, char** argv)
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;

    InetAddress listenAddr("127.0.0.1", 20000);

    dbdky::cma_server::cma_server_legacy server(&loop, listenAddr, "cma_server_legacy");
    server.start();
    loop.loop();

    return 0;
}
