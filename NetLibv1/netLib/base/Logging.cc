#include "Logging.h"
#include "Timestamp.h"
#include "CurrentThread.h"
#include "TimeZone.h"
namespace netLib
{
    __thread char t_errnobuf[512];
    __thread char t_time[64];
    __thread time_t t_lastSecond;
    // 将错误代码放入t_errnobuf
    const char *strerror_tl(int savedErrno)
    {
        return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
    }

    Logger::LogLevel initLogLevel()
    {
        if (::getenv("NETLIB_LOG_TRACE"))
            return Logger::TRACE;
        else if (::getenv("NETLIB_LOG_DEBUG"))
            return Logger::DEBUG;
        else
            return Logger::INFO;
    }

    Logger::LogLevel g_logLevel = initLogLevel();
    const char *LogLevelName[Logger::NUM_LOG_LEVELS] = {
        "TRACE ",
        "DEBUG ",
        "INFO  ",
        "WARN  ",
        "ERROR ",
        "FATAL ",
    };

    class T
    {
    public:
        T(const char *str, unsigned len) : str_(str), len_(len)
        {
            assert(sizeof(str_) == len_);
        }
        const char *str_;
        const unsigned len_;
    };

    inline LogStream &operator<<(LogStream &s, T v)
    {
        s.append(v.str_, v.len_);
        return s;
    }
    inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v)
    {
        s.append(v.data_, v.size_);
        return s;
    }

    // fwrite(msg -- 信息， 1 -- 信息大小， len -- 写入次数， stdout -- 写入的文件描述符)
    void defaultOuput(const char *msg, int len)
    {
        size_t n = fwrite(msg, 1, len, stdout);
        (void)n;
    }

    void defaultFlush()
    {
        fflush(stdout);
    }

    // g_output g_flush g_logTimeZone 这三个是线程全局变量, 所以在使用AsycnLogging.cc,
    // 如果是在多线程情况下， 必须先声明使用AsycnLogging， 然后替换掉g_output
    Logger::OutputFunc g_output = defaultOuput;
    Logger::FlushFunc g_flush = defaultFlush;
    TimeZone g_logTimeZone;

} //namespace netLib

using namespace netLib;

// if pass filename to Sourcefile, it would be error, becuase the Sourcefile have not default constructor
Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile &file, int line)
    : time_(Timestamp::now()), 
    stream_(),
    level_(level), 
    line_(line), 
    basename_(file)
{
    formatTime();
    CurrentThread::tid();
    stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
    stream_ << T(LogLevelName[level], 6);
    if (savedErrno != 0)
    {
        stream_ << strerror_tl(savedErrno) << "(errno = " << savedErrno << ") ";
    }
}

void Logger::Impl::formatTime()
{
    int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
    int microSeconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
    if (seconds != t_lastSecond)
    {
        // t_lastSecond 是全局的
        t_lastSecond = seconds;
        struct tm tm_time;
        if (g_logTimeZone.valid())
        {
            tm_time = g_logTimeZone.tolocaltime(seconds);
        }
        else
        {
            ::gmtime_r(&seconds, &tm_time);
        }
        // t_time 线程全局变量
        // 4+2+2+1+2+2+2 = 17
        int len = snprintf(t_time, sizeof(tm_time), "%4d%02d%02d %02d:%02d:%02d", tm_time.tm_year + 1900, tm_time.tm_mon+1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        assert(len == 17);
        (void)len;
    }
    if (g_logTimeZone.valid())
    {
        Fmt us(".%06d ", microSeconds);
        assert(us.length() == 8);
        stream_ << T(t_time, 17) << T(us.data(), 8);
    }
    else
    {
        Fmt us(".%06dZ ", microSeconds);
        assert(us.length() == 9);
        stream_ << T(t_time, 17) << T(us.data(), 9);
    }
}

void Logger::Impl::finish()
{
    stream_ << " - " << basename_ << ":" << line_ << "\n";
}

Logger::Logger(SourceFile file, int line)
    : impl_(INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char *func)
    : impl_(level, 0, file, line)
{
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line)
{
}

Logger::~Logger()
{
    impl_.finish();
    const LogStream::Buffer &buf(stream().buffer());
    g_output(buf.data(), buf.length());
    if (impl_.level_ == FATAL)
    {
        g_flush();
        abort();
    }
}

void Logger::setLogLevel(Logger::LogLevel level)
{
    g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
    g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
    g_flush = flush;
}

void Logger::setTimeZone(const TimeZone &tz)
{
    g_logTimeZone = tz;
}