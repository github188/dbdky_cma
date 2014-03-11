#ifndef __CMA_SESSION_IMAGETRANSFINISH_H_
#define __CMA_SESSION_IMAGETRANSFINISH_H_

//#include "cma_session_state.h"
#include "cma_session.h"	//xinsy20140310

namespace dbdky
{
namespace cma_server
{
class cma_session_imagetransfinish : public cma_session_state
{
public:
    void handleFrame(cma_frame_legacy* frame);
    Buffer* makeResponse();
};
}
}


#endif    //__CMA_SESSION_IMAGETRANSFINISH_H_
