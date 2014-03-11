#ifndef __CMA_SESSION_STATE_H_
#define __CMA_SESSION_STATE_H_
#include "cma_frame_legacy.h"

#include <port/Buffer.h>
#include <boost/shared_ptr.hpp>
#include "cma_session.h"	//xinsy20140310

using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_server
{
class cma_session;	
class cma_session_state
{
public:
    virtual void handleFrame(cma_frame_legacy* frame);
    virtual Buffer* makeResponse();

protected:
    void changeState(cma_session* s, cma_session_state* t)
    {
        s->changeState(t);
    }
};
}
}

#endif    //__CMA_SESSION_STATE_H_
