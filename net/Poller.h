#pragma once

#include <vector>
#include <map>

#include "../base/Timestamp.h"
#include "../net/EventLoop.h"

namespace netLib{
namespace net{

class Channel;
//
// Base class for multiplexing
//
// This class don't own the Channel

class Poller:noncopyable{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);

    virtual ~Poller();

    // polls the IO event
    // Must be called in the Loop thread
    virtual Timestamp poll(int timeouts, ChannelList* activeChannels) = 0;

    // Remove the channel, when it destructs;
    // must be called in the Loop thread
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    virtual bool hasChannel(Channel* channel);

    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const{
        ownerLoop_->assertInLoopThread();
    }

protected:
    typedef  std::map<int, Channel*> ChannleMap;
    ChannleMap channels_;

private:
    EventLoop* ownerLoop_;
};


}
}