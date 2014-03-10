#include "cma_sessionpool.h"

#include <boost/bind.hpp>
#include <assert.h>
#include <stdio.h>

namespace dbdky
{
namespace cma_server
{
cma_sessionpool::cma_sessionpool()
  : mutex_(),
    mutexSessions_(),
    running_(false)
{
}

cma_sessionpool::~cma_sessionpool()
{
    if (running_)
    {
        stop();
    }
}

void cma_sessionpool::start()
{
    assert(sessions_.empty());
    running_ = true;
}

void cma_sessionpool::stop()
{
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
    }
   
    {
        MutexLockGuard lock(mutexSessions_); 
        for_each(sessions_.begin(), sessions_.end(), 
            boost::bind(&dbdky::cma_server::cma_session::join, _1));
    }
}

void cma_sessionpool::handleInSession(const cma_frame_legacy& frame)
{
    {
        MutexLockGuard lock(mutexSessions_);
        boost::ptr_vector<cma_session>::iterator itr;
        for (itr = sessions_.begin(); itr != sessions_.end(); itr++)
        {
            if (itr->getSessionName() == frame.getDeviceId())
            {
                break;
            }
        }

        if (itr == sessions_.end())
        {
            sessions_.push_back(new cma_session(frame.getDeviceId()));
            itr = sessions_.end() - 1;
        }

        itr->handleInSession(frame);
    }
}

}
}
