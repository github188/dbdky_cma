#include "cma_client.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>
#include <utils/Timestamp.h>

namespace dbdky
{
namespace cma_client
{
    cma_client::cma_client(EventLoop* loop,
            const InetAddress& serverAddr,
            const string& name,
            uint16_t interval)
        : loop_(loop),
          name_(name),
          client_(loop, serverAddr, name),
          interval_(interval),
          threadPool_(new EventLoopThreadPool(loop))
    {
        threadPool_->setThreadNum(6);
        threadPool_->start();
    }

    cma_client::~cma_client()
    {
    }

    void cma_client::connect()
    {
        client_.connect();
    }

    void cma_client::disconnect()
    {
    }

    void cma_client::start()
    {
        systemTimer_ = loop_->runEvery(interval_,
            boost::bind(&cma_client::onTimer, this));

        connect();
    }

    void cma_client::stop()
    {
    }

    void cma_client::onTimer()
    {
        loop_->queueInLoop(
            boost::bind(&cma_client::sendTask, this));
    }

    void cma_client::sendTask()
    {
        LOG_INFO << name_ << ":" << "send";
        int sendLength;

        if (!getSendDataCallback_)
        {
            return;
        }

        boost::shared_ptr<uint8_t> buf = getSendDataCallback_(&sendLength);
        LOG_INFO << "***";
        for (int tmp = 0; tmp < sendLength; tmp++)
        {
            LOG_INFO << get_pointer(buf)[tmp] << "--";
        }
        write(sendLength, get_pointer(buf));
        disconnect();
    }

    void cma_client::write(const StringPiece& message)
    {
        MutexLockGuard lock(mutexConn_);
        if (connection_)
        {
            dbdky::port::Buffer buf;
            buf.append(message.data(), message.size());
            connection_->send(&buf);
        }
    }

    void cma_client::write(int length, uint8_t* buff)
    {
        MutexLockGuard lock(mutexConn_);

    }
}
}
