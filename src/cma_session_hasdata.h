#ifndef __CMA_SESSION_HASDATA_H_
#define __CMA_SESSION_HASDATA_H_

#include "cma_session_state.h"

namespace dbdky
{
namespace cma_server
{
class cma_session_hasdata : public cma_session_state
{
public:
    void handleFrame(const boost::shared_ptr<cma_frame_legacy>& frame);
    Buffer* makeResponse();
};
}
}

#endif    //__CMA_SESSION_HASDATA_H_
