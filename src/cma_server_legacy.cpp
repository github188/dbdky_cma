#include "cma_server_legacy.h"

#include <utils/Logging.h>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <utility>
#include <string>
#include <iostream>
#include <sstream>

#include "tinyxml.h"

using namespace std;

using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_server
{
cma_server_legacy::cma_server_legacy(dbdky::port::EventLoop* loop,
	const dbdky::port::InetAddress& listenAddr,
	const string& nameArg)
    : loop_(loop),
      server_(loop, listenAddr, "cma_server_legacy"),
      name_(nameArg)
{
    server_.setMessageCallback(
        boost::bind(&cma_server_legacy::onMessage, this, _1, _2));
}

void cma_server_legacy::start()
{
    server_.start();
}

void cma_server_legacy::onMessage(dbdky::port::Buffer* buf,
                		dbdky::Timestamp time)
{
    LOG_INFO << "**cma_server_legacy::onMessage ON TIME: " << time.toString();

}
}
}
