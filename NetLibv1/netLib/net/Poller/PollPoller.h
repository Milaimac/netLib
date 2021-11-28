/*
 * @Date: 2021-11-10 15:59:53
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-10 16:17:30
 * @FilePath: /netLib/net/Poller/PollPoller.h
 */

#pragma once

#include "../../net/Poller.h"
#include <vector>

struct pollfd;

namespace netLib
{
namespace net
{

///
/// IO Multiplexing with poll(2).
///
class PollPoller : public Poller
{
 public:

  PollPoller(EventLoop* loop);
  ~PollPoller() override;

  Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
  void updateChannel(Channel* channel) override;
  void removeChannel(Channel* channel) override;

 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  PollFdList pollfds_;
};

}  // namespace net
}  // namespace muduo