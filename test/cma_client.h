#ifndef __CMA_CLIENT_H_
#define __CMA_CLIENT_H_

#include <port/EventLoop.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <port/Callbacks.h>
#include <port/Buffer.h>
#include <utils/Timestamp.h>
#include <port/TcpClient.h>
#include <port/TcpConnection.h>

#include <port/EventLoop.h>
#include <port/EventLoopThreadPool.h>

using namespace dbdky;
using namespace dbdky::port;

namespace dbdky
{
namespace cma_client
{
    //typedef boost::function<int(uint8_t*)> GetSendDataCallback;
    typedef boost::function<boost::shared_ptr<uint8_t>(int*)> GetSendDataCallback;
    class cma_client : boost::noncopyable
    {
    public:
        void start();
        void stop();

        explicit cma_client(EventLoop* loop,
                const InetAddress& serverAddr,
                const string& name,
                uint16_t interval = 2);

        ~cma_client();
   
        void setConnectionCallback(const ConnectionCallback& cb)
        {
            connectionCallback_ = cb;
        }

        void setMessageCallback(const MessageCallback& cb)
        {
            messageCallback_ = cb;
        }

        void setWriteCompleteCallback(const WriteCompleteCallback& cb)
        {
            writeCompleteCallback_ = cb;
        }

        void setGetSendDataCallback(const GetSendDataCallback& cb)
        {
            getSendDataCallback_ = cb;
        }
    private:
        EventLoop* loop_;
        TimerId systemTimer_;
    
        const string name_;
    
        uint16_t interval_;

        TcpConnectionPtr connection_;
    
        boost::scoped_ptr<EventLoopThreadPool> threadPool_;
    
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        GetSendDataCallback getSendDataCallback_;

        void sendTask();
   
        void onTimer();
 
        TcpClient client_;

        void connect();
        void disconnect();
        void write(const StringPiece& message);
        void write(int length, uint8_t* buffer);
        
        void onConnection(const TcpConnectionPtr& conn);
        void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime);
        void onWriteComplete(const TcpConnectionPtr& conn);
 
        mutable MutexLock mutexConn_;
    };
}
}

#endif
