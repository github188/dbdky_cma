#ifndef __CMA_FRAME_LEGACY_H_
#define __CMA_FRAME_LEGACY_H_

#include <stdint.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>		//xinsy20140326

#include <string.h>

#include <string>
#include <map>

#include <utils/Logging.h>
#include <port/InetAddress.h>

#include "cma_callbacks.h"


using namespace std;

namespace dbdky
{
namespace cma_server
{
class cma_frame_legacy
{
public:
    enum CMA_UDP_FRM_TYPE
    {
        //
        CMA_UDP_FRM_MONIDATA = 0x01,
        CMA_UDP_FRM_DATARESP = 0x02,
        CMA_UDP_FRM_CTRL = 0x03,
        CMA_UDP_FRM_CTRLRESP = 0x04,
        CMA_UDP_FRM_HEART_BEAT = 0x05,
        CMA_UDP_FRM_IMAGEDATA = 0x06,
        CMA_UDP_FRM_IMAGECTRL = 0x07,
        CMA_UDP_FRM_SYNC = 0x08,
        CMA_UDP_FRM_UNKNOWN
    };

    enum CMA_UDP_PROTOCOL_TYPE
    {
        CMA_UDP_PTYPE_QX = 0x01,
	CMA_UDP_PTYPE_GTZD = 0x0b,
        CMA_UDP_PTYPE_GTQX = 0x0c,
        CMA_UDP_PTYPE_WFZD = 0x1f,
        CMA_UDP_PTYPE_DXHC = 0x20,
        CMA_UDP_PTYPE_DXWD = 0x21,
        CMA_UDP_PTYPE_FB_WEIGHT_MEATHD = 0x22,
	CMA_UDP_PTYPE_FB_ANGLE_MEATHD1 = 0x23,
	CMA_UDP_PTYPE_FB_ANGLE_MEATHD2 = 0x24,
        CMA_UDP_PTYPE_DXFP = 0x25,
        CMA_UDP_PTYPE_WD = 0x26,
        CMA_UDP_PTYPE_JJWD = 0x47,
        CMA_UDP_PTYPE_INSULATOR_FP = 0x5b,
	CMA_UDP_PTYPE_INSULATOR_WH = 0x5c,
	CMA_UDP_PTYPE_INSULATOR_XLDL = 0x5d,
	CMA_UDP_PTYPE_REQ_UP_IMAGE = 0xcc,						
	CMA_UDP_PTYPE_REMOTE_IMAGE = 0xcd,						
	CMA_UDP_PTYPE_REMOTE_IMAGE_UP_COMPLETE = 0xce,			
	CMA_UDP_PTYPE_REMOTE_IMAGE_COMPLEMENT = 0xcf,	
        CMA_UDP_PTYPE_UNKNOWN
    };

    cma_frame_legacy(CMA_UDP_FRM_TYPE ftype, CMA_UDP_PROTOCOL_TYPE ptype, 
        char deviceId[2], const uint8_t* pdudata, ssize_t pduLength, 
        dbdky::port::InetAddress& from);

    cma_frame_legacy(const cma_frame_legacy& frm);
    ~cma_frame_legacy();
    cma_frame_legacy& operator=(const cma_frame_legacy&);

    ssize_t getPduLength() const
    {
        return pduLength_;
    }

    uint8_t* getPduData() const
    {
        return pduData_;
    }

    void setParseFunc(const CmaLegacyFrameParserFunc& func)
    {
        parserFunc_ = func;
    }

    void dumpInfo() const;

    void parse()
    {
        if (!parserFunc_)
        {
            LOG_INFO << "Parser Missing";
            return;
        }

        monidata_.clear();
        monidata_ = parserFunc_(*this);
    }

    map<string,string> getMoniDataMap() const
    {
        return monidata_;
    }

    CMA_UDP_FRM_TYPE getFtype() const
    {
        return ftype_;
    }

    CMA_UDP_PROTOCOL_TYPE getPtype() const
    {
        return ptype_;
    }

    string getDeviceId() const
    {
        string ret(deviceId_);
        return ret;
    }

    void setCdid(const string& id)
    {
        cdid_ = id;
    }

    string getCdid() const
    {
        return cdid_;
    }

private:
    char deviceId_[2];
    ssize_t pduLength_;
    CMA_UDP_FRM_TYPE ftype_;
    CMA_UDP_PROTOCOL_TYPE ptype_;
    uint8_t* pduData_;
    CmaLegacyFrameParserFunc parserFunc_;
    map<string,string> monidata_;
    string cdid_;

    dbdky::port::InetAddress fromAddr;
};


}
}


#endif    //__CMA_FRAME_LEGACY_H_
