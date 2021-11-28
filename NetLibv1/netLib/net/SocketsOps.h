#pragma once
#include <arpa/inet.h>
// this SocketsOps need to implement this function 
// 1. accpet
// 2. connect 
// 3. bind
// 4. socket 
// 5. listen 
// 6. close
// 7. read  write
// 8. the imple_cast between sockaddr_in and  sockaddr  
// 9. shutdown  half-close
// 10. get information from addr

namespace  netLib{
namespace  net{
namespace  sockets{

int createNonBlockingorDie(sa_family_t family);
void listenOrDie(int sockfd);
void bindOrDie(int sockfd, const struct sockaddr* addr);
int accept(int sockfd, struct sockaddr_in6 *addr);      // sockaddr_in6 和 sockaddr_in的区别？
int connect(int sockfd, const struct sockaddr* addr);
ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);         // iovce 又是什么结构体
ssize_t write(int sockfd, const void *buf, size_t count);

void close(int sockfd);
void shutdownWrite(int sockfd);

void toIpPort(char* buf, size_t size, 
            const struct sockaddr* addr);
void toIp(char* buf, size_t size, 
            const struct sockaddr* addr);
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);
int getSocketError(int sockfd);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);
struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);


} //namespace sockets    
} // namespace  net
} // namespace netLib

// sockaddr and sockaddr_in 
// sockaddr_in is used to assign and definition
// sockaddr is used to become the parameter of function 