#include "cma_server.h"

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
#include "cma_ptlrender.h"
#include "ConfigUtil.h"

using namespace std;

namespace dbdky
{
namespace detail
{
#if 0 //xinsy20140213
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
#endif
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
	//xinsy20140213
	case 0xcc:
	{
		ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_REQ_UP_IMAGE;
		break;
	}
	case 0xcd:
	{
		ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_REMOTE_IMAGE;
		break;
	}
	case 0xce:
	{
		ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_REMOTE_IMAGE_UP_COMPLETE;
		break;
	}
	case 0xcf:
	{
		ptype = dbdky::cma_server::cma_frame::CMA_PTYPE_REMOTE_IMAGE_COMPLEMENT;
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
//make response packet //xinsy20140213
#if 1
uint16_t makeResponseFrame(bool ok,dbdky::cma_server::cma_frame frame,const uint8_t **serialData)
{
       #if 0
	dbdky::cma_server::cma_frame respFrm;
	respFrm.setPduLength(1);
	frame_.setPduLength(1);
	respFrm.setDeviceId(frame.getDeviceId());
	respFrm.setPtype(frame.getPtype());
	switch (frame.getFtype())
	{
	case dbdky::cma_server::cma_frame::CMA_FRM_MONIDATA:
		{
			respFrm.setFtype(dbdky::cma_server::cma_frame::CMA_FRM_DATARESP);

			break;
		}
	case dbdky::cma_server::cma_frame::CMA_FRM_CTRL:
		{
			respFrm.setFtype(dbdky::cma_server::cma_frame::CMA_FRM_CTRLRESP);
			break;
		}
	case dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUS:
		{
			respFrm.setFtype(dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUSRESP);
			break;
		}
	default:
		{
			//TODO:
			break;
		}
	}

	uint8_t* data = new uint8_t[1];

	if (ok)
	{
		data[0] = 0xFF;
		respFrm.setPduData(data);
	
	}
	else
	{
		data[0] = 0x00;
		respFrm.setPduData(data);
	}
     #endif

     #if 1
	uint16_t ret;
	uint8_t *tmpBuf;
        uint16_t length = 1;
	ret = length + 25;

	tmpBuf = new uint8_t[ret];

	tmpBuf[0] = 0xa5;
	tmpBuf[1] = 0x5a;
	tmpBuf[2] = (length<<8)&0xff;
	tmpBuf[3] = length&0xff;

	string 	deviceId;
        deviceId = frame.getDeviceId();
	const char* pDeviceId= deviceId.c_str();
        for(int tmp = 0;tmp<17;tmp++)
	{
	    tmpBuf[tmp+4]= pDeviceId[tmp];
	}
	
	switch (frame.getFtype())
	{
	case dbdky::cma_server::cma_frame::CMA_FRM_MONIDATA:
		{
			tmpBuf[21] = dbdky::cma_server::cma_frame::CMA_FRM_DATARESP;

			break;
		}
	case dbdky::cma_server::cma_frame::CMA_FRM_CTRL:
		{
			tmpBuf[21] = dbdky::cma_server::cma_frame::CMA_FRM_CTRLRESP;
			break;
		}
	case dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUS:
		{
			tmpBuf[21] = dbdky::cma_server::cma_frame::CMA_FRM_OPSTATUSRESP;
			break;
		}
	default:
		{
			//TODO:
			break;
		}
	}

	//tmpBuf[21] = ftype_;
	tmpBuf[22] = frame.getPtype();

	uint8_t* data = new uint8_t[1];

	if (ok)
	{
		tmpBuf[23] = 0xFF;
	
	}
	else
	{
		tmpBuf[23] = 0x00;
	}

	uint16_t crc = crc16(tmpBuf, length+23);
	tmpBuf[23 + length] = (crc<<8)&0xff;
	tmpBuf[24 + length] = crc&0xff;
	*serialData = tmpBuf;

	return ret;
     #endif

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
cma_server::cma_server(dbdky::port::EventLoop* loop,
            const dbdky::port::InetAddress& listenAddr,
            const string& nameArg)
    : loop_(loop),
      server_(loop, listenAddr, "cma_server"),
      name_(nameArg),
      dbhelper_(new DBHelper(ConfUtil::getInstance()->getDBPath(),
               ConfUtil::getInstance()->getDBUser(),
               ConfUtil::getInstance()->getDBPasswd(),
               ConfUtil::getInstance()->getDBName())),
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

string cma_server::getCdidOfFrame(const cma_frame& frm)
{
    string ret;
    string searchKey(frm.getDeviceId());

    map<string,string>::iterator itr;
    itr = cdiddirec_.find(searchKey);
    if (itr != cdiddirec_.end())
    {
        return itr->second;
    }

    string sql("SELECT OBJID, DeviceCode FROM sd_cd WHERE DeviceCode = \"");
    sql += frm.getDeviceId();
    sql += "\"";

    try
    {
        if (!dbhelper_->isConnected())
        {
            if (!dbhelper_->connect())
            {
                return string("");
            }
        }

        boost::shared_ptr<ResultSet> result(dbhelper_->query(sql));
       
        if (!result.get())
        {
            LOG_INFO << "NORESULT";
            return string("");
        }

        if (result->next())
        {
            string queryStr = boost::lexical_cast<string>(result->getInt("OBJID"));
      
            LOG_INFO << "QUERYSTR: " << queryStr;
            cdiddirec_.insert(make_pair<string,string>(searchKey,queryStr));
            ret += queryStr;
            return ret;
        }

        return ret;
    }
    catch (std::exception& e)
    {
        LOG_INFO << "Not found: " << e.what();
    } 
    
    return ret; 
}

void cma_server::onMessage(const dbdky::port::TcpConnectionPtr& conn,
		dbdky::port::Buffer* buf,
		dbdky::Timestamp time)
{
//    dbdky::string msg(buf->retrieveAllAsString());
//    LOG_INFO << conn->name() << "receive at [" << time.toString()
//		<< "]" << msg.size() << "'bytes: " << msg;
    LOG_INFO << "onmessage is called! "; //xinsy20131230
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
            buf->retrieve(parselength);
            CmaFrameParserFunc func = cma_ptlrender::getInstance()->renderParser(frame);
            frame.setParseFunc(func);
            //tmpLoop->runInLoop(boost::bind(&cma_frame::parse, frame)); 
            //loop_->runInLoop(boost::bind(&cma_frame::parse, frame));
            frame.parse();
 
            frame.setCdid(getCdidOfFrame(frame)); 
            CmaSqlInsertStringMaker maker = cma_ptlrender::getInstance()->renderSqlInsertStringMaker(frame);
            string insertStr = maker(frame);
            
			LOG_INFO <<"???" << insertStr;
            LOG_INFO << "CMA_FRM_OK";

			//xinsy20140213 the server give a reply to cilent as a response  when the server receive the correct frame 
		#if 1
			const uint8_t *data;
            uint16_t len = detail::makeResponseFrame(true,frame,&data);
			LOG_INFO << "The response frame 's length is "<<len;
			string dataStr((const char*)data);
			LOG_INFO << "The response frame is  "<<data;
			conn->send(data,len);
		#endif
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
