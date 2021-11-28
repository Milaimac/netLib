#pragma once

#include "../base/StringPiece.h"
#include "../base/copyable.h"

#include <netinet/in.h>

namespace netLib{
namespace net
{
namespace sockets
{// in order to avoid #include, we define here first
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);     
} // namespace sockets
class InetAddress:netLib::copyable{
public:
    // loopbackOnly means it only in 127.0.0.1
    explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
    InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);
    explicit InetAddress(const struct sockaddr_in& addr)
    :addr_(addr){}
    explicit InetAddress(const struct sockaddr_in6& addr)
    :addr6_(addr){}

    sa_family_t family() const {return addr_.sin_family; }
    string toIp() const;
    string toIpPort() const;
    uint16_t port() const;
    const struct sockaddr* getSockAddr() const{return sockets::sockaddr_cast(&addr6_);}
    void setSockAddrInet6(const struct sockaddr_in6& addr6){addr6_ = addr6; }
    uint32_t ipv4NetEndian() const;
    uint16_t portNetEndian() const {return addr_.sin_port; }
    // resolve to --- 解析至
    // resolve hostname to IP address, not changing port or sin_family
    // return true on success.
    // thread safe  
    static bool resolve(StringArg hostname, InetAddress* result);
    void setScopeId(uint32_t scope_id);
private:
    union{
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
};
} // namespace net

}