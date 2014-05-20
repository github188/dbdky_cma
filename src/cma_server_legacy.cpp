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

#include "crc.h"
#include "util.h"
#include "cma_ptlrender_legacy.h"

using namespace std;

namespace dbdky
{
namespace detail
{

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
  
    if ((NULL == sourceData) || (length < 10))
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
   
    uint16_t tmpLength = dbdky::makeword(buffer[6], buffer[7]);
    if (tmpLength + 10 > length)
    {
        ret = CMA_FRM_BAD_FRAME;
        *frameLength = length;
        delete [] buffer;
        return ret;
    } 

    uint16_t tmpCRC = dbdky::makeword(buffer[10 + tmpLength-2], buffer[10 + tmpLength-1]);

    if (tmpCRC != crc16(buffer, 10-2 + tmpLength))
    {
        ret = CMA_FRM_CRC;
        *frameLength = 10-2 + tmpLength;
        delete [] buffer;
        return ret;
    }

    *frameLength = tmpLength + 10;
    ret = CMA_FRM_OK; 
   
    delete [] buffer; 
    return ret;
}


dbdky::cma_server::cma_frame_legacy validBuf2Frame(uint8_t* buffer)
{
    char deviceId[2];

    int16_t length = dbdky::makeword(buffer[6], buffer[7]);

    dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_TYPE ftype;
    dbdky::cma_server::cma_frame_legacy::CMA_UDP_PROTOCOL_TYPE ptype;

    for (int tmp = 0; tmp < 2; tmp++)
    {
        deviceId[tmp] = buffer[2 + tmp];
    }

    switch (buffer[4])
    {
        case 0x01:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_MONIDATA;    
            break;
        }
        case 0x02:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_DATARESP;    
            break;
        }
        case 0x03:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRL;    
            break;
        }
        case 0x04:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRLRESP;    
            break;
        }
        case 0x05:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_HEART_BEAT;       
            break;
        }
        case 0x06:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_IMAGEDATA;    
            break;
        }
        case 0x07:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_IMAGECTRL;    
            break;
        }
        case 0x08:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_SYNC;    
            break;
        }
        default:
        {
            ftype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_UNKNOWN;
        }
    } 

    switch (buffer[5])
    {
        case 0x01:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_QX;
            break;
        }
        case 0x0c:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_GTZD;
            break;
        }
        case 0x1e:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_GTQX;
            break;
        }
        case 0x1f:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_WFZD;
            break;
        }
        case 0x20:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_DXHC;
            break;
        }
        case 0x21:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_DXWD;
            break;
        }
        case 0x22:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_FB_WEIGHT_MEATHD;
            break;
        }
        case 0x23:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD1;
            break;
        }
        case 0x24:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_FB_ANGLE_MEATHD2;
            break;
        }
        case 0x25:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_DXFP;
            break;
        }
	case 0x26:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_WD;
            break;
        }
	case 0x47:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_JJWD;
            break;
        }
	case 0x5b:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_FP;
            break;
        }
        case 0x5c:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_WH;
            break;
        }
	case 0x5d:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_INSULATOR_XLDL;
            break;
        }
	case 0xcc:
	{
		ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_REQ_UP_IMAGE;
		break;
	}
	case 0xcd:
	{
		ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE;
		break;
	}
	case 0xce:
	{
		ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_UP_COMPLETE;
		break;
	}
	case 0xcf:
	{
		ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_REMOTE_IMAGE_COMPLEMENT;
		break;
	}
        default:
        {
            ptype = dbdky::cma_server::cma_frame_legacy::CMA_UDP_PTYPE_UNKNOWN;
        }
    }

    uint8_t *pdudata = new uint8_t[length];
    memcpy(pdudata, &buffer[8], length);
    dbdky::cma_server::cma_frame_legacy ret(ftype, ptype, deviceId, pdudata, length);

    delete [] pdudata;

    return ret;
}
#if 0
uint16_t makeResponseFrame(bool ok,dbdky::cma_server::cma_frame_legacy frame,const uint8_t **serialData)
{
	uint16_t ret;
	uint8_t *tmpBuf;
        uint16_t length = 1;
	ret = length + 10;

	tmpBuf = new uint8_t[ret];

	tmpBuf[0] = 0xa5;
	tmpBuf[1] = 0x5a;
	tmpBuf[6] = (length<<8)&0xff;
	tmpBuf[7] = length&0xff;

	string 	deviceId;
        deviceId = frame.getDeviceId();
	const char* pDeviceId= deviceId.c_str();
        for(int tmp = 0;tmp<2;tmp++)
	{
	    tmpBuf[tmp+2]= pDeviceId[tmp];
	}
	
	switch (frame.getFtype())
	{
	case dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_MONIDATA:
		{
			tmpBuf[4] = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_DATARESP;
			break;
		}
	case dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRL:
		{
			tmpBuf[4] = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRLRESP;
			break;
		}
	default:
		{
			//TODO:
			break;
		}
	}

	tmpBuf[5] = frame.getPtype();

	uint8_t* data = new uint8_t[1];

	if (ok)
	{
		tmpBuf[8] = 0xFF;
	
	}
	else
	{
		tmpBuf[8] = 0x00;
	}

	uint16_t crc = crc16(tmpBuf, length+10-2);
	tmpBuf[10-2 + length] = (crc<<8)&0xff;
	tmpBuf[10-1 + length] = crc&0xff;
	*serialData = tmpBuf;

	return ret;

}
#endif

}
}


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

#if 1
//xinsy20140327
uint16_t cma_server_legacy::makeResponseFrame(bool ok,const cma_frame_legacy &frame,const uint8_t **serialData)
{
	uint16_t ret;
	uint8_t *tmpBuf;
        uint16_t length = 1;
	ret = length + 10;

	tmpBuf = new uint8_t[ret];

	tmpBuf[0] = 0xa5;
	tmpBuf[1] = 0x5a;
	tmpBuf[6] = length&0xff;	//xinsy20140327
	tmpBuf[7] = (length>>8)&0xff;   //xinsy20140327

	string 	deviceId;
        deviceId = frame.getDeviceId();
	const char* pDeviceId= deviceId.c_str();
        for(int tmp = 0;tmp<2;tmp++)
	{
	    tmpBuf[tmp+2]= pDeviceId[tmp];
	}
	
	switch (frame.getFtype())
	{
	case dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_MONIDATA:
		{
			tmpBuf[4] = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_DATARESP;
			break;
		}
	case dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRL:
		{
			tmpBuf[4] = dbdky::cma_server::cma_frame_legacy::CMA_UDP_FRM_CTRLRESP;

			break;
		}
	default:
		{

			//TODO:
			break;
		}
	}


	tmpBuf[5] = frame.getPtype();

	uint8_t* data = new uint8_t[1];

	if (ok)
	{
		tmpBuf[8] = 0xFF;
	
	}
	else
	{
		tmpBuf[8] = 0x00;
	}

	uint16_t crc = crc16(tmpBuf, length+10-2);
	tmpBuf[10-2 + length] = crc&0xff;	//xinsy20140327

	tmpBuf[10-1 + length] = (crc>>8)&0xff;	//xinsy20140327
	*serialData = tmpBuf;

	return ret;


}
#endif

void cma_server_legacy::onMessage(dbdky::port::Buffer* buf,
                		dbdky::Timestamp time)
{
    LOG_INFO << "**cma_server_legacy::onMessage ON TIME: " << time.toString();

}
}
}
