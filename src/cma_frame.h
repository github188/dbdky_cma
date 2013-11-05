#ifndef __CMA_FRAME_H_
#define __CMA_FRAME_H_

#include <stdint.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string.h>

#include <string>
#include <map>

#include <utils/Logging.h>

#include "cma_callbacks.h"

using namespace std;

namespace dbdky
{
namespace cma_server
{
class cma_frame
{
public:
    enum CMA_FRM_TYPE
    {
        //According to Q/GDW 242-2010 C.8.1
        CMA_FRM_MONIDATA = 0x01,
        CMA_FRM_DATARESP = 0x02,
        CMA_FRM_CTRL = 0x03,
        CMA_FRM_CTRLRESP = 0x04,
        CMA_FRM_REMOTEIMAGEDATA = 0x05,
        CMA_FRM_REMOTEIMAGECTRL = 0x06,
        CMA_FRM_OPSTATUS = 0x07,
        CMA_FRM_OPSTATUSRESP = 0x08,
        CMA_FRM_SYNC = 0x09,
        CMA_FRM_UNKNOWN
    };

    enum CMA_PROTOCOL_TYPE
    {
        CMA_PTYPE_QX = 0x01,
        CMA_PTYPE_GT = 0x0c,
        CMA_PTYPE_ZDTZ = 0x1e,
        CMA_PTYPE_ZDBX = 0x1f,
        CMA_PTYPE_DXHC = 0x20,
        CMA_PTYPE_WD = 0x21,
        CMA_PTYPE_FB = 0x22,
        CMA_PTYPE_FP = 0x23,
        CMA_PTYPE_WDTZ = 0x24,
        CMA_PTYPE_WDGJ = 0x25,
        CMA_PTYPE_WH = 0x5c,
        CMA_PTYPE_UNKNOWN
    };

    cma_frame(CMA_FRM_TYPE ftype, CMA_PROTOCOL_TYPE ptype, char deviceId[17], const uint8_t* pdudata, ssize_t pduLength);

    cma_frame(const cma_frame& frm);
    ~cma_frame();
    cma_frame& operator=(const cma_frame&);

    ssize_t getPduLength() const
    {
        return pduLength_;
    }

    uint8_t* getPduData() const
    {
        return pduData_;
    }

    void setParseFunc(const CmaFrameParserFunc& func)
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

    CMA_FRM_TYPE getFtype() const
    {
        return ftype_;
    }

    CMA_PROTOCOL_TYPE getPtype() const
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
    char deviceId_[17];
    ssize_t pduLength_;
    CMA_FRM_TYPE ftype_;
    CMA_PROTOCOL_TYPE ptype_;
    uint8_t* pduData_;
    CmaFrameParserFunc parserFunc_;
    map<string,string> monidata_;
    string cdid_;
};


}
}


#endif    //__CMA_FRAME_H_
