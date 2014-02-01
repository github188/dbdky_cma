#include <utils/Logging.h>

#include <utils/ThreadPool.h>

#include <boost/bind.hpp>

using namespace dbdky;

void Task()
{
    LOG_INFO << "Task";
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
