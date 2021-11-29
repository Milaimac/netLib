#include "../net/Acceptor.h"

#include "../base/Logging.h"
#include "../net/EventLoop.h"
#include "../net/InetAddress.h"
#include "../net/SocketsOps.h"

// #include <unistd.h>
#include <fcntl.h>
using namespace netLib;
using namespace netLib::net;


// notice the "/dev/null" is null device, is a special device file, it abandon all data write in, it will get you EOF if you read it
Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
    :loop_(loop), 
    acceptSocket_(sockets::createNonBlockingorDie(listenAddr.family())),
    acceptChannel_(loop, acceptSocket_.fd()),
    listening_(false),
    idleFd_(::open("/dev/null", O_RDONLY|O_CLOEXEC))
{
    assert(idleFd_ >= 0);
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(
        std::bind(&Acceptor::handleRead, this)
    );
}

Acceptor::~Acceptor(){
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen(){
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead(){
    loop_->assertInLoopThread();
    InetAddress peerAddr;
    // FIXME must consider the condition that fd is used until no more
    int connfd =  acceptSocket_.accept(&peerAddr);
    if(connfd >= 0)
    {
        if(newconncetionCallback_){
            newconncetionCallback_(connfd, peerAddr);
        }
        else
        {
            sockets::close(connfd);
        }
    }
    else
    {
        LOG_SYSERROR << "in Acceptor::handleRead ";
        
    }
}
