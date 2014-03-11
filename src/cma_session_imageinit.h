#ifndef __CMA_SESSION_IMAGEINIT_H_
#define __CMA_SESSION_IMAGEINIT_H_

//#include "cma_session_state.h"	//xinsy20140310
#include "cma_session.h"		//xinsy20140310

namespace dbdky
{
namespace cma_server
{
class cma_session_imageinit : public cma_session_state
{
public:
    void handleFrame(cma_frame_legacy* frame);
    Buffer* makeResponse();
};
}
}


#endif    //__CMA_SESSION_IMAGEINIT_H_
