/*
 * @Author: your name
 * @Date: 2021-11-05 11:09:34
 * @LastEditTime: 2021-11-28 11:31:53
 * @LastEditors: kafier
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /netLib/net/Poller.h
 */
#pragma once

#include <vector>
#include <map>

#include "../base/Timestamp.h"
#include "../net/EventLoop.h"
// #include "netLib/net/EventLoop.h"

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

    virtual bool hasChannel(Channel* channel) const;

    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const{
        ownerLoop_->assertInLoopThread();
    }

protected:
    typedef  std::map<int, Channel*> ChannelMap;
    ChannelMap channels_;

private:
    EventLoop* ownerLoop_;
};


}
}