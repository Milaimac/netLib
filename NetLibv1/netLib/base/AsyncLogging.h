#include "Logstream.h"
#include "Thread.h"
#include "CountDownLatch.h"

// #include "Mutex.h"       redefintion

#include <memory>
#include <atomic>
#include <vector>

namespace netLib
{
    class AsyncLogging : noncopyable
    {
    public:
        AsyncLogging(const std::string &basename,
                     off_t rollsize,
                     int flushInterval = 3);
        ~AsyncLogging()
        {
            if (running_)
            {
                stop();
            }
        }

        void append(const char *logline, int len);
        void start()
        {
            running_ = true;
            thread_.start();            // thread.start() 中latch.wait()
            latch_.wait();
        }

        void stop()
        {
            running_ = false;
            cond_.notify();
            thread_.join();
        }



    private:
        void threadFunc();

        typedef netLib::detail::FixedBuffer<detail::kLargeBuffer> Buffer;
        typedef std::vector<std::unique_ptr<Buffer>> BufferVector;

        typedef BufferVector::value_type BufferPtr;

        const int flushInterval_;
        std::atomic<bool> running_;
        const std::string basename_;
        const off_t rollSize_;
        netLib::Thread thread_;
        netLib::CountDownLatch latch_;
        netLib::Mutex mutex_;
        netLib::Condition cond_;
        BufferPtr currentbuffer_;
        BufferPtr nextbuffer_;
        
        BufferVector buffers_;
    };
}