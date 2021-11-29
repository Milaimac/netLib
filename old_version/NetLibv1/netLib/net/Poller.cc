/*
 * @Date: 2021-11-10 15:49:26
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-10 16:15:57
 * @FilePath: /netLib/net/Poller.cc
 */
#include "../net/Poller.h"
#include "../net/Channel.h"

using namespace netLib;
using namespace netLib::net;

Poller::Poller(EventLoop* loop)
:ownerLoop_(loop){

}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const{
    assertInLoopThread();
    ChannelMap::const_iterator it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}