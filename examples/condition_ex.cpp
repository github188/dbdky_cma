#include <utils/Logging.h>
#include <utils/Condition.h>
#include <utils/Thread.h>

#include <boost/bind.hpp>

#include <unistd.h>

using namespace dbdky;

MutexLock mutex;
Condition cond(mutex);

void ThreadFunc2()
{
    LOG_INFO << "ThreadFunc2";
    
    for (int tmp = 0; tmp < 5; tmp++)
    {
        LOG_INFO << "ThreadFunc2 Sleep: " << tmp;
        sleep(1);
    }

    cond.notify();
}

int main(int argc, char** argv)
{
    Thread t2(boost::bind(&ThreadFunc2));

    t2.start();

    while (1)
    {
        cond.wait();
        break;
    }

    LOG_INFO << "Main Loop exit";

    return 0;
}
