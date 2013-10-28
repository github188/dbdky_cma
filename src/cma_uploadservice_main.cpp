#include "cma_uploadservice.h"

#include <utils/Logging.h>
#include <port/EventLoop.h>

using namespace dbdky;
using namespace dbdky::port;
using namespace dbdky::cma_server;

int main(int argc, char** argv)
{
    LOG_INFO << " pid = " << getpid();
    EventLoop loop;
    cma_uploadservice service(&loop, "cma_uploadservice");
    service.start();
    loop.loop();
}
