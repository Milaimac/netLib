#include "../net/InetAddress.h"

#include "../base/Logging.h"
#include "../net/Endian.h"
#include "../net/SocketsOps.h"

#include <netdb.h>
#include <netinet/in.h>

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

using namespace netLib;
using namespace netLib::net;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6), 
            "InetAddress is same size as sockaddr_in6");
static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_port offset 2");
// this is allocate the ip address and port
InetAddress::InetAddress(uint16_t portArg, bool loopbackOnly, bool ipv6){
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_offset 0");
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_offset 0");
    if(ipv6){
        memZero(&addr6_, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;// here can directly assgin the ip
        addr6_.sin6_port = sockets::hostToNetwork16(portArg);
    }
    else{
        memZero(&addr_, sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
        addr_.sin_port = sockets::hostToNetwork16(portArg);
    }
}

string InetAddress::toIpPort() const{
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof(buf), getSockAddr());
    return buf;
}

string InetAddress::toIp() const{
    char buf[64] = "";
    sockets::toIp(buf, sizeof(buf), getSockAddr());
    return buf;
}

// what means the function 
uint32_t InetAddress::ipv4NetEndian() const{
    assert(family() == AF_INET);
    return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::port() const{
    return sockets::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(StringArg hostname, InetAddress* out){
    assert(out != NULL);
    struct hostent hent;
    struct hostent*  he = nullptr;
    int herrno = 0;
    memZero(&hent, sizeof(hent));
    // if gethostbyname_r success, it will store result to hent, and he is the pointer of hent
    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof(t_resolveBuffer), &he, &herrno);
    if(ret == 0  && he != nullptr){
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);       // h_addr is h_adddrlist[0]
        return true;
    }
    else{
        if(ret){
            LOG_SYSERROR << "InetAddress::resolve";
        }
        return false;
    }
}

void InetAddress::setScopeId(uint32_t scope_id){
    if(family() == AF_INET6){
        addr6_.sin6_scope_id = scope_id;
    }
}