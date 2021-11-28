/*
 * @Date: 2021-11-10 15:07:23
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-10 16:05:09
 * @FilePath: /netLib/net/Poller/DefaultPoller.cc
 */
#include "../../net/Poller.h"
#include "../../net/Poller/EpollPoller.h"
#include "../../net/Poller/PollPoller.h"
#include <stdlib.h>

using namespace netLib::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("MUDUO_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
