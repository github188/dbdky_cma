#include "cma_client.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <utils/Logging.h>
#include <utils/Timestamp.h>
#include <src/util.h>		//xinsy20140320
#include <src/crc.h>		//xinsy20140320


namespace dbdky
{

const uint16_t CMA_PACKET_HEADER = 0x5aa5;
#if 1
enum FrameParseErr
{
    CMA_FRM_OK = 0,
    CMA_FRM_INSUFFICIENT,
    CMA_FRM_BAD_FRAME,
    CMA_FRM_CRC,
    CMA_FRM_UNKNOWN
};
#endif
FrameParseErr client_checkCMAFrame(Buffer* buf, int* frameLength)
{
    FrameParseErr ret;
    const char* sourceData = buf->peek();
    int length = buf->readableBytes();
  
    if ((NULL == sourceData) || (length < 25))
    {
        ret = CMA_FRM_INSUFFICIENT;
        *frameLength = length;
        return ret; 
    } 
     
    uint8_t* buffer = new uint8_t[length];
    memcpy(buffer, sourceData, length);

    uint16_t tmpHeader = makeword(buffer[0], buffer[1]);
    if (tmpHeader != CMA_PACKET_HEADER)
    {
        ret = CMA_FRM_BAD_FRAME;
        *frameLength = length;
        delete [] buffer;
        return ret; 
    }
   
    uint16_t tmpLength = makeword(buffer[2], buffer[3]);
    if (tmpLength + 25 > length)
    {
        ret = CMA_FRM_BAD_FRAME;
        *frameLength = length;
        delete [] buffer;
        return ret;
    } 

    uint16_t tmpCRC = makeword(buffer[23 + tmpLength], buffer[24 + tmpLength]);

    if (tmpCRC != crc16(buffer, 23 + tmpLength))
    {
        ret = CMA_FRM_CRC;
        *frameLength = 23 + tmpLength;
        delete [] buffer;
        return ret;
    }

    *frameLength = tmpLength + 25;
    ret = CMA_FRM_OK; 
   
    delete [] buffer; 
    return ret;
}

}




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
        client_.setConnectionCallback(
            boost::bind(&cma_client::onConnection, this, _1));
        client_.setMessageCallback(
            boost::bind(&cma_client::onMessage, this, _1, _2, _3));
        client_.setWriteCompleteCallback(
            boost::bind(&cma_client::onWriteComplete, this, _1));

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
        int sendLength;
        if (!getSendDataCallback_)
        {
            LOG_INFO << "NULL getSendDataCallback_";
            return;
        }

        boost::shared_ptr<uint8_t> buf = getSendDataCallback_(&sendLength);
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

    void cma_client::onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> "
            << conn->peerAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");

        MutexLockGuard lock(mutexConn_);
        if (conn->connected())
        {
            connection_ = conn;
        }
        else
        {
            connection_.reset();
        }
    }

    void cma_client::onMessage(const TcpConnectionPtr& conn,
                      Buffer* buf,
                      Timestamp receiveTime)
    {
        LOG_INFO << "onMessage";
	//xinsy20140320
	int parselength;
    	FrameParseErr res;
    	res = client_checkCMAFrame(buf, &parselength);

	switch (res)
    	{
		case CMA_FRM_OK:
		{
		    LOG_INFO << "CMA_FRM_OK";
		    break;
		}
		case CMA_FRM_INSUFFICIENT:
		{
		    LOG_INFO << "CMA_FRM_INSUFFICIENT";
		    break;
		}
		case CMA_FRM_BAD_FRAME:
		{
		    LOG_INFO << "CMA_FRM_BAD_FRAME";
		    buf->retrieve(parselength);
		    break;
		}
		case CMA_FRM_CRC:
		{
		    LOG_INFO << "CMA_FRM_CRC ERROR";
		    buf->retrieve(parselength);
		    break;
		}
		case CMA_FRM_UNKNOWN:
		{
		    LOG_INFO << "CMA_FRM_UNKNOWN";
		    buf->retrieve(parselength);
		    break;
		}
		default:
		{
		    LOG_INFO << "default";
		    buf->retrieve(parselength);
		    break;
		}
	}

        string response = buf->retrieveAllAsString();
        LOG_INFO << response;
    }

    void cma_client::onWriteComplete(const TcpConnectionPtr& conn)
    {
    }

    void cma_client::write(int length, uint8_t* buff)
    {
        if ((length <= 0) || (NULL == buff))
        {
            return;
        }

        MutexLockGuard lock(mutexConn_);
        if (connection_)
        {
            dbdky::port::Buffer buf;
            buf.append(buff, length);
            connection_->send(&buf);
        }
    }
}
}
