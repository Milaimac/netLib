#pragma once
#include <string>
#include "noncopyable.h"

namespace netLib{
class AppendFile:noncopyable{
public:
    // muduo 使用 的是stringPiece的stringArg
    explicit AppendFile(std::string filename);      

    ~AppendFile();

    void append(const char *logline, const size_t len);
    // 由于需要记录使用了多少字节，以便实现文件回滚
    off_t writtenBytes() const {return writtenBytes_; }
    void flush();
private:
    size_t write(const char* logline, size_t len);

    FILE *fp_;

    char buffer_[64 * 1024];
    off_t writtenBytes_;
};
}