#ifndef __CMA_SESSION_WAIT_H_
#define __CMA_SESSION_WAIT_H_

//#include "cma_session_state.h"

#include "cma_session.h"	//xinsy20140310

namespace dbdky
{
namespace cma_server
{
class cma_session_wait : public cma_session_state
{
public:
    void handleFrame(cma_frame_legacy* frame);
    Buffer* makeResponse();
};
}
}


#endif    //__CMA_SESSION_WAIT_H_
