#pragma once

#include "../base/Atomic.h"
#include "../base/Types.h"

#include "../net/TcpConnection.h"

#include <map>

namespace netLib
{
    namespace net
    {
        // 前向声明
        class Acceptor;
        class EventLoop;
        class EventLoopThreadPool;

        //
        // Tcp server, supports single-threaded and thread pool models;
        //
        // This is an interface class, so don't expose too much details

        class TcpServer : noncopyable
        {
        public:
            typedef std::function<void(EventLoop *)> ThreadInitCallback;
            enum Option
            {
                kNoReusePort,
                kReusePort
            };

            TcpServer(EventLoop *loop,
                      const InetAddress &listenAddr,
                      const string &nameArg,
                      Option option = kNoReusePort);
            ~TcpServer();

            const string &ipPort() const { return ipPort_; }

            const string &name() const { return name_; }

            EventLoop *getLoop() const { return loop_; }

            /// Set the number of threads for handling input.
            ///
            /// Always accepts new connection in loop's thread.
            /// Must be called before @c start
            /// @param numThreads
            /// - 0 means all I/O in loop's thread, no thread will created.
            ///   this is the default value.
            /// - 1 means all I/O in another thread.
            /// - N means a thread pool with N threads, new connections
            ///   are assigned on a round-robin basis.

            void setThreadNum(int numThreads);

            void setTheadInitCallback(const ThreadInitCallback &cb)
            {
                threadInitCallback_ = cb;
            }
            // valid after calling start()
            std::shared_ptr<EventLoopThreadPool> threadPool()
            {
                return threadPool_;
            }

            // start the server if it's not listening
            //
            // It's hramless to call it multiple times
            // Thread safe
            void start();

            // set connectionCallback_ , not Thread safe
            void setConnectionCallback(const ConnectionCallback &cb)
            {
                connectionCallback_ = cb;
            }
            // set messageCallback, not Thread safe
            void setMessageCallback(const MessageCallback &cb)
            {
                messageCallback_ = cb;
            }
            // set Wirtecomplete Callback, not thread safe
            void setWriteCompleteCallback(const WriteCompleteCallback &cb)
            {
                writeCompleteCallback_ = cb;
            }

        private:
            // Not threadsafe, but in loop
            void newConnection(int sockfd, const InetAddress &peerAddr);
            // threadSafe
            void removeConnection(const TcpConnectionPtr &conn);
            // Not Thread safe, but in loop
            void removeConnecitonInLoop(const TcpConnectionPtr &conn);

            typedef std::map<string, TcpConnectionPtr> ConnectionMap;

            EventLoop *loop_;

            // the ipPort is the listen addr's ip port
            const string ipPort_;
            const string name_;
            std::unique_ptr<Acceptor> acceptor_; //avoid revealing Acceptor
            std::shared_ptr<EventLoopThreadPool> threadPool_;
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            ThreadInitCallback threadInitCallback_;
            AtomicInt32 started_;
            // always in loop thread
            int nextConnId_;
            ConnectionMap connections_;
        };

    }
}
