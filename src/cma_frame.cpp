#include "cma_frame.h"

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
    {
        memcpy(get_pointer(pduData_), pdudata, pduLength);
        memcpy(deviceId_, deviceId, 17);
    }
}
}
