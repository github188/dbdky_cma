#ifndef __CMA_SESSIONPOOL_H_
#define __CMA_SESSIONPOOL_H_

#include <utils/Mutex.h>
#include <utils/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "cma_frame_legacy.h"
#include "cma_session.h"

namespace dbdky
{
namespace cma_server
{
class cma_sessionpool : boost::noncopyable
{
public:
    explicit cma_sessionpool();
    ~cma_sessionpool();

    void start();
    void stop();

private:
    void handleInSession(const cma_frame_legacy& frame);
    
    MutexLock mutex_;
    MutexLock mutexSessions_;
    boost::ptr_vector<cma_session> sessions_;

    bool running_;
};
}
}


#endif    //__CMA_SESSIONPOOL_H_
