#include <utils/Logging.h>
#include <utils/Thread.h>

#include <unistd.h>
#include <iostream>

#include <boost/bind.hpp>

using namespace dbdky;
using namespace std;

void ThreadFunc1()
{
    LOG_INFO << "Start ThreadFunc 1";
    for (int tmp = 0; tmp < 5; tmp++)
    {
        LOG_INFO << "ThreadFunc1 Sleep: " << tmp;
        sleep(1.5);
    }
}

void ThreadFunc2()
{
    LOG_INFO << "Start Thread Func 2";
    for (int tmp = 0; tmp < 5; tmp++)
    {
        LOG_INFO << "ThreadFunc2 Sleep: " << tmp;
        sleep(1);
    }
}

int main(int argc, char** argv)
{
    Thread t1(boost::bind(&ThreadFunc1), "thread1");
    Thread t2(boost::bind(&ThreadFunc2), "thread2");
    LOG_INFO << "numCreated: " << Thread::numCreated() << " t1: name = " << t1.name() << " tid = " << t1.tid() << " started():" << (t1.started() ? "STARTED" : "NOT YET");

    LOG_INFO << "numCreated: " << Thread::numCreated() << " t2: name = " << t2.name() << " tid = " << t2.tid() << " started():" << (t2.started() ? "STARTED" : "NOT YET");
    t1.start();
    t2.start();

    t1.join();
    t2.join();


    LOG_INFO << "numCreated: " << Thread::numCreated() << " t1: name = " << t1.name() << " tid = " << t1.tid() << " started():" << (t1.started() ? "STARTED" : "NOT YET");

    LOG_INFO << "numCreated: " << Thread::numCreated() << " t2: name = " << t2.name() << " tid = " << t2.tid() << " started():" << (t2.started() ? "STARTED" : "NOT YET");

    return 0;
}
