#include <utils/Logging.h>

#include <utils/ThreadPool.h>

#include <boost/bind.hpp>

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

using namespace dbdky;

void Task()
{
    LOG_INFO << "Task: tid = " << pthread_self();
}

int main(int argc, char** argv)
{
    ThreadPool pool("TestThreadPool");
    pool.start(15);
    for (int tmp = 0; tmp < 100; tmp++)
    {
        pool.run(boost::bind(&Task));
    }
    return 0;
}
