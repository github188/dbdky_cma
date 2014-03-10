#ifndef __CMA_SESSION_H_
#define __CMA_SESSION_H_

#include <utils/Thread.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "cma_session_state.h"
#include "cma_frame_legacy.h"

#include <utils/Condition.h>
#include <utils/Mutex.h>

namespace dbdky
{
namespace cma_server
{
class cma_session_state;
class cma_session : boost::noncopyable
{
public:
    friend class cma_session_state;
    typedef boost::function<void ()> ThreadFunc;

    explicit cma_session(const string& name = string());
    virtual ~cma_session();
    const string& getSessionName() const;
    
    void start();
    int join();

    bool started() const;

    void handleInSession(const cma_frame_legacy&);
private:
    cma_session_state* state_;
    void changeState(cma_session_state*); 
    
private:
    void idleFunc();
    boost::shared_ptr<Thread> thread_;
    
    const string name_;
    Condition message_;
    MutexLock mutex_;
    cma_frame_legacy* pending_frame;
};
}
}


#endif    //__CMA_SESSION_H_
