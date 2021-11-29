#pragma  once
#include "../base/noncopyable.h"

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;
namespace  netLib{
namespace  net{

class InetAddress;
// wrapper of socket file descriptor
// 
// it close the sockfd when destructs
// it's thread safe, all operations are delagated to os

class Socket : noncopyable{
public:
    explicit Socket(int sockfd)
        :sockfd_(sockfd){}
    ~Socket();
    int fd() const {return sockfd_;}
    // return true if succes
    bool getTcpInfo(struct tcp_info*) const;
    bool getTcpInfoString(char* buf, int len) const;
    // abort if addr in use
    void bindAddress(const InetAddress& localaddr);
    // abort if addr in use
    void listen();
    /// On success, returns a non-negative integer that is
    /// a descriptor for the accepted socket, which has been
    /// set to non-blocking and close-on-exec. *peeraddr is assigned.
    /// On error, -1 is returned, and *peeraddr is untouched.
    int accept(InetAddress* peeraddr);
    void shutdownWrite();
    // Enable/disable TCP_NODELAY
    void setTcpNoDelay(bool on);
    // Enable/disable SO_REUSEADDR
    void setReuseAddr(bool on);
    // Enable/disable SO_REUSEPORT
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int sockfd_;
};
}

}