#pragma once
#include "Logstream.h"
#include "Timestamp.h"
// #include "CurrentThread.h"

namespace netLib
{
    // class AsyncLogging;     //前向声明
    class TimeZone;
    class Logger
    {
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };

        class SourceFile
        {
        public:
            template <int N>
            // strrchr 会找到最后被'/'分割的字符串首地址。 /usr/boot/file ，将会找到file
            SourceFile(const char (&arr)[N])
                : data_(arr), 
                size_(N - 1)
            {
                const char *slash = strrchr(data_, '/');
                if (slash)
                {
                    data_ = slash + 1;
                    size_ -= static_cast<int> (data_ - arr);
                }
                // size_ -= static_cast<int>(data_ - slash);
            }

            explicit SourceFile(const char *filename)
                : data_(filename)
            {
                const char *slash = strchr(filename, '/');
                if (slash)
                {
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }
            // these data member should be public
            const char *data_;
            int size_;
        };
        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, LogLevel level, const char *func);
        Logger(SourceFile file, int line, bool toAbort);

        ~Logger();

        LogStream &stream() { return impl_.stream_; }

        static LogLevel logLevel();

        static void setLogLevel(LogLevel level);

        typedef void (*OutputFunc)(const char *msg, int len);
        typedef void (*FlushFunc)();

        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);
        static void setTimeZone(const TimeZone &tz);

    private:
        class Impl
        {
        public:
            typedef Logger::LogLevel LogLevel;
            // Impl(const char* filename, int line);
            Impl(LogLevel level, int old_errno, const SourceFile &file, int line);

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

    inline Logger::LogLevel Logger::logLevel()
    {
        return g_logLevel;
    }

#define LOG_TRACE                                            \
    if (netLib::Logger::logLevel() <= netLib::Logger::TRACE) \
    netLib::Logger(__FILE__, __LINE__, netLib::Logger::TRACE, __func__).stream()
#define LOG_DEBUG                                            \
    if (netLib::Logger::logLevel() <= netLib::Logger::DEBUG) \
    netLib::Logger(__FILE__, __LINE__, netLib::Logger::DEBUG, __func__).stream()

#define LOG_INFO                                            \
    if (netLib::Logger::logLevel() <= netLib::Logger::INFO) \
    netLib::Logger(__FILE__, __LINE__, netLib::Logger::INFO).stream()

#define LOG_WARN netLib::Logger(__FILE__, __LINE__, netLib::Logger::WARN).stream()
#define LOG_ERROR netLib::Logger(__FILE__, __LINE__, netLib::Logger::ERROR).stream()
#define LOG_FATAL netLib::Logger(__FILE__, __LINE__, netLib::Logger::FATAL).stream()
#define LOG_SYSERROR netLib::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL netLib::Logger(__FILE__, __LINE__, true).stream()

    const char *strerror_tl(int savedErrno);

// Check that the input is non Null, This very useful in constructor
// initializer lists
#define CHECKNOTNULL(val) \
    ::netLib::CheckNotNull(__FILE__, __LINE__, "'" #val "'Must be Not NULL", (val))

    template <typename T>
    T *CheckNotNull(Logger::SourceFile file, int line, const char *names, T *ptr)
    {
        if (ptr == NULL)
        {
            Logger(file, line, Logger::FATAL).stream() << names;
        }
        return ptr;
    }
}
