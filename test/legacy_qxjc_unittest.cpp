#include "cma_client.h"

#include <utils/Logging.h>
#include <port/EventLoop.h>
#include <port/InetAddress.h>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

using namespace dbdky;
using namespace dbdky::port;


#define SENDBUF_SIZE	40
const uint8_t sendbuf[SENDBUF_SIZE] = {0xa5, 0x5a,    //sync Header
	0x01, 0x00,		//netbasestation_id
	0x01,			//frame type
	0x01,			//packet type
	0x1e, 0x00,		//packet length
        0x01, 0x00,		//sensor_id
	0x21, 0x02, 0x13, 0x20,	//time
	0x01, 0x00,		//windSpeed
	0x02, 0x00,		//windDirection
	0x03, 0x00,		//Air_Temperature
	0x04, 0x00,		//Humidity
	0x05, 0x00, 		//Air_Pressure
	0x06, 0x00, 		//Precipitation_Intensity
        0x07, 0x00,		//Radiation_Intensity
	0x08, 0x00,		//daily_rainfall
	0x09, 0x00,		//battery_voltage
	0x10, 0x00,		//operation_temperture

	0x00, 0x00,		//Reserve1
	0x00, 0x00, 		//Reserve2

	0x28,
	0xc7
	
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
