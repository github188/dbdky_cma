#include "cma_session.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace dbdky
{
namespace cma_server
{
cma_session::cma_session(const string& name)
    : name_(name),
      thread_(new Thread(
	boost::bind(&dbdky::cma_server::cma_session::idleFunc, this), name))
{
}

cma_session::~cma_session()
{
    if (thread_->started())
    {
        thread_->join();
    }

    if (NULL != state_)
    {
        delete state_;
        state_ = NULL;
    }
}

const string& cma_session::getSessionName() const
{
    return name_;
}

void cma_session::start()
{
    if (!thread_->started())
    {
        thread_->start();
    }
}

int cma_session::join()
{
    //Here we have some risk.
    //If thread_ has been joined and we call thread_.join.
    //Under debug mode, Thread::join() have a assert(!joined_)
    //In that case, we will have assert fail.
    if (thread_->started())
    {
        return thread_->join();
    }

    return 0;
}

bool cma_session::started() const
{
    return thread_->started();
}

void cma_session::handleInSession(const cma_frame_legacy& frm) const
{
    //TODO:
}

void cma_session::idleFunc()
{
    //TODO:
}

}
}
