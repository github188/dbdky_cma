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
                ssize_t pduLength)
      : ftype_(ftype),
        ptype_(ptype),
        pduLength_(pduLength),
        pduData_(new uint8_t[pduLength])
        //parserFunc_(defaultParseFunc)
   {
        setParseFunc(UdpdefaultParseFunc);
        memcpy(pduData_, pdudata, pduLength);
        memcpy(deviceId_, deviceId, 2);
    }

    cma_frame_legacy::cma_frame_legacy(const cma_frame_legacy& frm)
        : ftype_(frm.ftype_),
          ptype_(frm.ptype_),
          pduLength_(frm.pduLength_)
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

    void cma_frame_legacy::dumpInfo() const
    {
    }

}
}
