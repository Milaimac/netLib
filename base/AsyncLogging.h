#include "Logstream.h"
#include "Thread.h"
#include "CountDownLatch.h"

// #include "Mutex.h"       redefintion

#include <memory>
#include <atomic>
#include <vector>


namespace  netLib{
class AsyncLogging:noncopyable{
public:
    AsyncLogging(const std::string& basename, 
                off_t rollsize,
                int flushInterval = 3);
    ~AsyncLogging(){
        if(running_){
            stop();
        }
    }

    void start(){
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop(){
        running_ = false;
        cond_.notify();
        thread_.join();
    }

    void append(const char* logline, int len);
private:
    void threadFunc();

    typedef netLib::FixedBuffer<KLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;

    typedef  BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    const std::string basename_;
    const off_t rollSize_;

    netLib::Mutex mutex_;
    netLib::CountDownLatch latch_;
    netLib::Thread thread_;
    netLib::Condition cond_;
    BufferPtr nextbuffer_;
    BufferPtr currentbuffer_;
    BufferVector buffers_;


};
}