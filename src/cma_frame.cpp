#include "cma_frame.h"

#include <boost/bind.hpp>

namespace dbdky
{
namespace cma_server
{
    cma_frame::cma_frame(CMA_FRM_TYPE ftype,
		CMA_PROTOCOL_TYPE ptype,
		char deviceId[17],
                const uint8_t* pdudata,
                ssize_t pduLength)
      : ftype_(ftype),
        ptype_(ptype),
        pduLength_(pduLength),
        pduData_(new uint8_t[pduLength])
        //parserFunc_(defaultParseFunc)
   {
        setParseFunc(defaultParseFunc);
        memcpy(pduData_, pdudata, pduLength);
        memcpy(deviceId_, deviceId, 17);
    }

    cma_frame::cma_frame(const cma_frame& frm)
        : ftype_(frm.ftype_),
          ptype_(frm.ptype_),
          pduLength_(frm.pduLength_)
    {
        parserFunc_ = frm.parserFunc_;
        pduData_ = new uint8_t[pduLength_];
        memcpy(pduData_, frm.pduData_, pduLength_);
    }

    cma_frame& cma_frame::operator=(const cma_frame& frm)
    {
        if (&frm == this)
        {
            return *this;
        }

        ftype_ = frm.ftype_;
        ptype_ = frm.ptype_;
        pduLength_ = frm.pduLength_;
        parserFunc_ = frm.parserFunc_;
	memcpy(deviceId_,frm.deviceId_,17);//xinsy20140324
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

    cma_frame::~cma_frame()
    {
        if (pduData_)
        {
            delete [] pduData_;
            pduData_ = NULL;
        }
    }
    //xinsy20140326
    void cma_frame::dumpInfo() const
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
