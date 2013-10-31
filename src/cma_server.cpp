#include "cma_server.h"

#include <utils/Logging.h>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include "tinyxml.h"

#include "crc.h"

namespace dbdky
{
namespace detail
{
uint16_t crc16(uint8_t *puchMsg, uint16_t usDataLen)
{
    uint8_t uchCRCHi = 0xff;
    uint8_t uchCRCLo = 0xff;
    uint16_t uIndex;

    while (usDataLen--)
    {
        uIndex = uchCRCHi ^ *puchMsg++;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }

    return (uchCRCHi << 8 | uchCRCLo);
}

const uint16_t CMA_PACKET_HEADER = 0x5aa5;

uint16_t makeword(uint8_t low, uint8_t high)
{
    uint16_t ret = 0;
    ret |= high;
    ret <<= 8;
    ret |= low;

    return ret;
     
}

enum FrameParseErr
{
    CMA_FRM_OK = 0,
    CMA_FRM_INSUFFICIENT,
    CMA_FRM_BAD_FRAME,
    CMA_FRM_CRC,
    CMA_FRM_UNKNOWN
};

FrameParseErr checkCMAFrame(dbdky::port::Buffer* buf, int* frameLength)
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
}


namespace dbdky
{
namespace cma_server
{
cma_server::cma_server(dbdky::port::EventLoop* loop,
            const dbdky::port::InetAddress& listenAddr,
            const string& nameArg)
    : loop_(loop),
      server_(loop, listenAddr, "cma_server"),
      name_(nameArg)
{
    server_.setConnectionCallback(
        boost::bind(&cma_server::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&cma_server::onMessage, this, _1, _2, _3));
}

void cma_server::start()
{
    server_.start();
}

void cma_server::onConnection(const dbdky::port::TcpConnectionPtr& conn)
{
    LOG_INFO << "cma_server - " << conn->peerAddress().toIpPort() << " ->  "
	<< conn->localAddress().toIpPort() << " is "
 	<< (conn->connected() ? "UP" : "DOWN");
}

void cma_server::onMessage(const dbdky::port::TcpConnectionPtr& conn,
		dbdky::port::Buffer* buf,
		dbdky::Timestamp time)
{
//    dbdky::string msg(buf->retrieveAllAsString());
//    LOG_INFO << conn->name() << "receive at [" << time.toString()
//		<< "]" << msg.size() << "'bytes: " << msg;
    //TODO:
    int parselength;
    detail::FrameParseErr res;
    res = detail::checkCMAFrame(buf, &parselength);

    switch (res)
    {
        case detail::CMA_FRM_OK:
        {
            boost::shared_ptr<uint8_t> framebuffer(new uint8_t[parselength]);
            memcpy(get_pointer(framebuffer), buf->peek(), parselength); 
            LOG_INFO << "CMA_FRM_OK";
            break;
        }
        case detail::CMA_FRM_INSUFFICIENT:
        {
            LOG_INFO << "CMA_FRM_INSUFFICIENT";
            break;
        }
        case detail::CMA_FRM_BAD_FRAME:
        {
            LOG_INFO << "CMA_FRM_BAD_FRAME";
            buf->retrieve(parselength);
            break;
        }
        case detail::CMA_FRM_CRC:
        {
            LOG_INFO << "CMA_FRM_CRC";
            buf->retrieve(parselength);
            break;
        }
        case detail::CMA_FRM_UNKNOWN:
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
}

}
}
