#include "../net/Buffer.h"
#include "../net/SocketsOps.h"

using namespace netLib;
using namespace netLib::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSzie;

ssize_t Buffer::readFd(int fd, int* savedErrno){
    char extrabuf[64*1024];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin()+writableBytes();
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    // when  there is enough space in this buffer, don't read into extarbuf,
    // when extrabuf is used, we read 128k-1 at most
    const int iovcnt = (writable < sizeof(extrabuf)) ? 2: 1;
    const ssize_t n = sockets::readv(fd, vec, iovcnt);
    if(n < 0){
        *savedErrno = errno;
    }
    else if(implicit_cast<size_t>(n) <= writable){
        writeIndex_ += n;
    }
    else{
        writeIndex_ = buffer_.size();
        append(extrabuf, n-writable);
    }
    return n;
}