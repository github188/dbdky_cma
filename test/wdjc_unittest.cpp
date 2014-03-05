#include "cma_client.h"

#include <utils/Logging.h>
#include <port/EventLoop.h>
#include <port/InetAddress.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

using namespace dbdky;
using namespace dbdky::port;


#define SENDBUF_SIZE	64

const uint8_t sendbuf[SENDBUF_SIZE] = {0xa5, 0x5a,    //Header
	0x27, 0x00,//Data Length
	0x32,0x31,0x4d,0x30,0x36,0x30,0x31,0x4a,0x43,0x30,0x30, 0x30, 0x30, 0x30, 0x30, 0x30,0x31, //CMD_ID
	0x01,	//
	0x21,	//
	0x32,0x31,0x4d,0x30,0x36,0x30,0x31,0x4a,0x43,0x30,0x30, 0x30, 0x30, 0x30, 0x30, 0x30,0x32,//component_ID
	0x4d, 0x21, 0x3f, 0x51,//time
	0x01, 0x02,		//alarm_flag
	0x78, 0x56, 0x34, 0x12,//m_Line_Temperature1
	0x78, 0x56, 0x34, 0x13,//m_Line_Temperature2
	0x78, 0x56, 0x34, 0x14,//Reserve1
	0x78, 0x56, 0x34, 0x15,//Reserve2
	0x44,
	0x4e
	
};

boost::shared_ptr<uint8_t> getTestData(int* length)
{
    *length = SENDBUF_SIZE;
    boost::shared_ptr<uint8_t> ret(new uint8_t[SENDBUF_SIZE]);
    for (int tmp = 0; tmp < SENDBUF_SIZE; tmp++)
    {
        memcpy(get_pointer(ret), sendbuf, SENDBUF_SIZE);
    } 

    return ret;
}



int main()
{
    EventLoop loop;
    InetAddress proxyAddr("127.0.0.1", 6001);
    dbdky::cma_client::cma_client client(&loop, proxyAddr, "cma_client");
    client.setGetSendDataCallback(boost::bind(getTestData, _1));
    client.start();
    loop.loop();	
}
