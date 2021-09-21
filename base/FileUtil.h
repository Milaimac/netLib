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
    //TODO off_t writtenBytes() const {return writtenByters_; }
    void flush();
private:
    size_t write(const char* logline, size_t len);

    FILE *fp_;

    char buffer_[64 * 1024];
    //TODO off_t writenBytes_;
};
}