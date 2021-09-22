#pragma once
#include "noncopyable.h"
#include "FileUtil.h"
#include "Mutex.h"
#include "Types.h"
#include <memory>
namespace netLib{
class LogFile:noncopyable{
public:
    LogFile(const std::string& basename, 
            off_t rollsize, 
            bool threadsafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    
    ~LogFile();

    void append(const char* logline, int len); //将日志logline加到（append）logfile
    void flush();
    bool rollFile();
private:
    void append_unlocked(const char* logline, int len);
    static string getLogFilename(const string& basename, time_t* now);
    const std::string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveryN_;

    int count_;
    std::unique_ptr<Mutex> mutex_;
    time_t startOfperiod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<AppendFile> file_;

    const int kRollPerSeconds_ = 60 * 60 * 24;      

};
}