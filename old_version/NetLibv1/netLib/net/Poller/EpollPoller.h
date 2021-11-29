/*
 * @Date: 2021-11-10 15:09:18
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-10 15:14:44
 * @FilePath: /netLib/net/Poller/EpollPoller.h
 */
#pragma once
#include "../../net/Poller.h"
#include <vector>

struct epoll_event;

namespace netLib
{
    namespace net
    {
        class EPollPoller : public Poller
        {
        public:
            EPollPoller(EventLoop *loop);

            ~EPollPoller() override;

            Timestamp poll(int timeouts, ChannelList *activeChannels) override;

            void updateChannel(Channel *channel) override;

            void removeChannel(Channel *channel) override;

        private:
            static const int kInitEventListSize = 16;
            static const char *operationToString(int op);

            void fillActiveChannels(int numEvents,
                                    ChannelList *activeChannles) const;
            void update(int operation, Channel *channel);

            typedef std::vector<struct epoll_event> EventList;

            int epollfd_;

            EventList events_;
        };
    } //namespace net
} //namespace NetLib