#include<boost/noncopyable.hpp>

class EventLoop:boost::noncopyable{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void assertInLoopThread(){
        if(!isInloopThread()){
            abortNotInLoopThread();
        }
    }

    bool isInloopThread() const {return threadId_;}

private:
    void abortNotInLoopThread();

    bool looping_;      //atomic
    const pid_t threadId_;
};