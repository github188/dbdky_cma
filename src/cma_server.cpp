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
#include "util.h"
#include "cma_frame.h"
#include "cma_ptlrender.h"

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

    uint16_t tmpHeader = dbdky::makeword(buffer[0], buffer[1]);
    if (tmpHeader != CMA_PACKET_HEADER)
    {
        ret = CMA_FRM_BAD_FRAME;
        *frameLength = length;
        delete [] buffer;
        return ret; 
    }
   
    uint16_t tmpLength = dbdky::makeword(buffer[2], buffer[3]);
    if (tmpLength + 25 > length)
    {
        ret = CMA_FRM_BAD_FRAME;
        *frameLength = length;
        delete [] buffer;
        return ret;
    } 

    uint16_t tmpCRC = dbdky::makeword(buffer[23 + tmpLength], buffer[24 + tmpLength]);

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


dbdky::cma_server::cma_frame validBuf2Frame(uint8_t* buffer)
{
    char deviceId[17];

    int16_t length = dbdky::makeword(buffer[2], buffer[3]);

    dbdky::cma_server::cma_frame::CMA_FRM_TYPE ftype;
    dbdky::cma_server::cma_frame::CMA_PROTOCOL_TYPE ptype;

    for (int tmp = 0; tmp < 17; tmp++)
    {
        deviceId[tmp] = buffer[4 + tmp];
    }

    switch (buffer[21])
    {
        case 0x01:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_MONIDATA;    
            break;
        }
        case 0x02:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_DATARESP; 
            break;
        }
        case 0x03:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_CTRL;    
            break;
        }
        case 0x04:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_CTRLRESP; 
            break;
        }
        case 0x05:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_REMOTEIMAGEDATA;    
            break;
        }
        case 0x06:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_REMOTEIMAGECTRL; 
            break;
        }
        case 0x07:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUS;    
            break;
        }
        case 0x08:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUSRESP;
            break;
        } 
        case 0x09:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_SYNC; 
            break;
        }
        default:
        {
            ftype = dbdky::cma_server::cma_frame::CMA_FRM_UNKNOWN;
        }
    } 

    switch (buffer[22])
    {
        case 0x01:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_QX;
            break;
        }
        case 0x0c:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_GT;
            break;
        }
        case 0x1e:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_ZDTZ;
            break;
        }
        case 0x1f:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_ZDBX;
            break;
        }
        case 0x20:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_DXHC;
            break;
        }
        case 0x21:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_WD;
            break;
        }
        case 0x22:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_FB;
            break;
        }
        case 0x23:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_FP;
            break;
        }
        case 0x24:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_WDTZ;
            break;
        }
        case 0x25:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_WDGJ;
            break;
        }
        case 0x5c:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_WH;
            break;
        }
        default:
        {
            ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_UNKNOWN;
        }
    }

    uint8_t *pdudata = new uint8_t[length];
    memcpy(pdudata, &buffer[23], length);
    dbdky::cma_server::cma_frame ret(ftype, ptype, deviceId, pdudata, length);

    delete [] pdudata;

    return ret;
}
}
}

using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_server
{
cma_server::cma_server(dbdky::port::EventLoop* loop,
            const dbdky::port::InetAddress& listenAddr,
            const string& nameArg)
    : loop_(loop),
      server_(loop, listenAddr, "cma_server"),
      name_(nameArg),
      threadPool_(new EventLoopThreadPool(loop))
{
    server_.setConnectionCallback(
        boost::bind(&cma_server::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&cma_server::onMessage, this, _1, _2, _3));
    threadPool_->setThreadNum(20);
    threadPool_->start();
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
    int parselength;
    detail::FrameParseErr res;
    res = detail::checkCMAFrame(buf, &parselength);

    switch (res)
    {
        case detail::CMA_FRM_OK:
        {
            uint8_t* framebuffer = new uint8_t[parselength];
            memcpy(framebuffer, buf->peek(), parselength);
            cma_frame frame = detail::validBuf2Frame(framebuffer);
            delete [] framebuffer;
            //frame.dumpInfo();
            buf->retrieve(parselength);
	    EventLoop* tmpLoop = threadPool_->getNextLoop();
            if (NULL == tmpLoop)
            {
                return;
            }
            CmaFrameParserFunc func = cma_ptlrender::getInstance()->renderParser(frame);
            frame.setParseFunc(func);
            tmpLoop->runInLoop(boost::bind(&cma_frame::parse, frame)); 
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
