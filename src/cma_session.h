#ifndef __CMA_SESSION_H_
#define __CMA_SESSION_H_

#include <utils/Thread.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "cma_session_state.h"
#include "cma_frame_legacy.h"

namespace dbdky
{
namespace cma_server
{
class cma_session : boost::noncopyable
{
public:
    typedef boost::function<void ()> ThreadFunc;

    explicit cma_session(const string& name = string());
    virtual ~cma_session();
    const string& getSessionName() const;
    
    void start();
    int join();

    bool started() const;

    void handleInSession(const cma_frame_legacy&) const;
protected:
    cma_session_state* state_;

private:
    void idleFunc();
    boost::shared_ptr<Thread> thread_;
    
    const string name_;
};
}
}


#endif    //__CMA_SESSION_H_
