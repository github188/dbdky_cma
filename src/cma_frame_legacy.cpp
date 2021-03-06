#include "cma_frame_legacy.h"

#include <boost/bind.hpp>

namespace dbdky
{
namespace cma_server
{
    cma_frame_legacy::cma_frame_legacy(CMA_UDP_FRM_TYPE ftype,
		CMA_UDP_PROTOCOL_TYPE ptype,
		char deviceId[2],
                const uint8_t* pdudata,
                ssize_t pduLength,
                dbdky::port::InetAddress& addr)
      : ftype_(ftype),
        ptype_(ptype),
        pduLength_(pduLength),
        pduData_(new uint8_t[pduLength]),
        fromAddr(addr)
        //parserFunc_(defaultParseFunc)
   {
        setParseFunc(UdpdefaultParseFunc);
        memcpy(pduData_, pdudata, pduLength);
        memcpy(deviceId_, deviceId, 2);
    }

    cma_frame_legacy::cma_frame_legacy(const cma_frame_legacy& frm)
        : ftype_(frm.ftype_),
          ptype_(frm.ptype_),
          pduLength_(frm.pduLength_),
          fromAddr(frm.fromAddr)
    {
        parserFunc_ = frm.parserFunc_;
        pduData_ = new uint8_t[pduLength_];
        memcpy(pduData_, frm.pduData_, pduLength_);
    }

    cma_frame_legacy& cma_frame_legacy::operator=(const cma_frame_legacy& frm)
    {
        if (&frm == this)
        {
            return *this;
        }

        ftype_ = frm.ftype_;
        ptype_ = frm.ptype_;
        pduLength_ = frm.pduLength_;
        parserFunc_ = frm.parserFunc_;
        fromAddr = frm.fromAddr;
	memcpy(deviceId_, frm.deviceId_, 2);		//xinsy20140326
        if (frm.pduData_)
        {
            if (pduData_)
            {
                delete [] pduData_;
            }

            pduData_ = new uint8_t[pduLength_];
            memcpy(pduData_, frm.pduData_, pduLength_);
        }

        return *this;
    }

    cma_frame_legacy::~cma_frame_legacy()
    {
        if (pduData_)
        {
            delete [] pduData_;
            pduData_ = NULL;
        }
    }
    //xinsy20140326
    void cma_frame_legacy::dumpInfo() const
    {
	string strFtype = boost::lexical_cast<string>(ftype_);
	string strPtype = boost::lexical_cast<string>(ptype_);
	string strPduData = boost::lexical_cast<string>(pduData_);
	string strDeviceId = boost::lexical_cast<string>(deviceId_);
	LOG_INFO<<"deviceId:"<<strDeviceId;
	LOG_INFO<<"ftype:"<<strFtype;
	LOG_INFO<<"ptype:"<<strPtype;
	LOG_INFO<<"PduData:"<<strPduData;
    }

}
}
