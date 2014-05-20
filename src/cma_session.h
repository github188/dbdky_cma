#ifndef __CMA_SESSION_H_
#define __CMA_SESSION_H_

#include <utils/Thread.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

//#include "cma_session_state.h"			//xinsy20140310
#include "cma_frame_legacy.h"

#include <utils/Condition.h>
#include <utils/Mutex.h>

#include <port/Buffer.h>				//xinsy20140310

using namespace dbdky;					//xinsy20140310
using namespace dbdky::port;				//xinsy20140310


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
    //friend class cma_session_state;			//xinsy20140310
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
//xinsy20140310
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


#endif    //__CMA_SESSION_H_
