#include <utils/Logging.h>
#include <port/Buffer.h>

#include <string.h>

using namespace dbdky;
using namespace dbdky::port;

int main(int argc, char** argv)
{
    Buffer buf;
    LOG_INFO << "Before OP. buf [readableBytes=" << buf.readableBytes()
        << "] [writableBytes=" << buf.writableBytes() << "]"
        << " [prependableBytes=" << buf.prependableBytes() << "]";
        

    buf.append("hello world", strlen("hello world"));
    
    LOG_INFO << "After appending \"hello world\". buf [readableBytes=" << buf.readableBytes()
        << "] [writableBytes=" << buf.writableBytes() << "]"
        << " [prependableBytes=" << buf.prependableBytes() << "]";

    LOG_INFO << "RETRIEVEALLASSTRING:" << buf.retrieveAllAsString();
    
    buf.appendInt8(1);
    buf.appendInt16(10);
    buf.appendInt32(100);
    
   
    int8_t a = buf.readInt8();
    int16_t b = buf.readInt16();
    int32_t c = buf.readInt32();

    LOG_INFO << "Read: [" << a << "]"
        << "[" << b << "]"
        << "[" << c << "]";

    return 0;
}
