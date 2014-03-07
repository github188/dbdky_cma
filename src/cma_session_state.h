#ifndef __CMA_SESSION_STATE_H_
#define __CMA_SESSION_STATE_H_

#include "cma_frame_legacy.h"

#include <port/Buffer.h>
#include <boost/shared_ptr.hpp>

using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_server
{
class cma_session_state
{
public:
    virtual void handleFrame(const boost::shared_ptr<cma_frame_legacy>& frame);
    virtual Buffer* makeResponse();
};
}
}


#endif    //__CMA_SESSION_STATE_H_
