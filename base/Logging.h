#pragma once
#include "Logstream.h"
#include "Timestamp.h"
#include "CurrentThread.h"
namespace netLib{
// class AsyncLogging;     //前向声明
class TimeZone;
class Logger{
public:
    enum LogLevel{
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    class SourceFile{
    public:
        template<int N>
        // strrchr 会找到最后被'/'分割的字符串首地址。 /usr/boot/file ，将会找到file
        SourceFile(const char (&arr)[N])
        :data_(arr), size_(N-1)
        {
            const char* slash = strrchr(data_, '/');
            if(slash){
                data_ = slash+1;
            }
            size_ -= static_cast<int>(data_ - slash);
        }

        explicit SourceFile(const char* filename)
        :data_(filename)
        {   
            const char* slash = strchr(filename, '/');
            if(slash){
                data_ = slash+1;
            }
            size_ = static_cast<int>(strlen(data_));
        }
        // these data member should be public
        const char* data_;
        int size_;
    };
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level,  const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    
    ~Logger();

    LogStream &stream() {return impl_.stream_; }

    static LogLevel logLevel();

    static void setLogLevel(LogLevel level);

    typedef void(*OutputFunc)(const char* msg, int len);
    typedef void(*FlushFunc)();

    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    static void setTimeZone(const  TimeZone& tz);

private:
class Impl{
public:
    typedef Logger::LogLevel LogLevel;
    // Impl(const char* filename, int line);
    Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

    void formatTime();
    void finish();

    Timestamp time_;
    LogStream stream_;
    LogLevel level_;
    int line_;

    // std::string  basename_;
    SourceFile basename_;
};
    Impl impl_;
};
extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel(){
    return g_logLevel;
}

//TODO  

}
#define LOG Logger(__FILE__, __LINE__).stream()