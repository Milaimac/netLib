#include "../base/LogFile.h"
#include "../base/FileUtil.h"
#include "../base/ProcessInfo.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>
using namespace netLib;

LogFile::LogFile(const string& basename, 
                off_t rollSize,
                bool threadsSafe,
                int flushInterval, 
                int checkEveryN)
:basename_(basename), 
rollSize_(rollSize), 
flushInterval_(flushInterval), 
checkEveryN_(checkEveryN),
count_(0), 
mutex_(threadsSafe ? new Mutex: NULL),
startOfperiod_(0), 
lastRoll_(0),
lastFlush_(0)
{
    assert(basename.find('/') == string::npos);
    rollFile();
}

LogFile::~LogFile()=default;

void LogFile::append(const char* logline, int len){
    if(mutex_){
        MutexLockGuard lock(*mutex_);
        append_unlocked(logline, len);
    }
    else{
        append_unlocked(logline, len);
    }
}

void LogFile::flush(){
    if(mutex_){
        MutexLockGuard lock(*mutex_);
        file_->flush();
    }
    else{
        file_->flush();
    }
}

void LogFile::append_unlocked(const char* logline, int len){
    file_->append(logline, len);

    if(file_->writtenBytes() > rollSize_){
        rollFile();
    }
    else{// writtenBytes <=  rollSize 没有到达规定的文件大小，但是达到规定的时间， 这里规定的是24小时。 这样的话，下面的又乘又除倒是可以理解了
        ++count_;
        if(count_ >= checkEveryN_){// 只不过这里的检查是根据checkEveryN的
            count_ = 0;
            time_t now = ::time(NULL);
            time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
            if(thisPeriod_ != startOfperiod_){      
                rollFile();
            }
            else if(now - lastFlush_ > flushInterval_){
                lastFlush_ = now;
                file_->flush();
            }
        }
    }
}

bool LogFile::rollFile(){
    time_t now = 0;
    string filename = getLogFilename(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

    if(now > lastRoll_){
        lastRoll_ = now;
        lastFlush_ = now;
        startOfperiod_ = start;
        file_.reset(new AppendFile(filename));
        return true;
    }
    return false;
}

string LogFile::getLogFilename(const string& basename, time_t* now){
    string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;
    
    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(timebuf, sizeof timebuf, "%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    filename += ProcessInfo::hostname();

    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
    filename += pidbuf;

    filename += ".log";
    return filename;
}