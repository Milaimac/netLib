set(net_SRCS
    "./Acceptor.cc"
    "./Buffer.cc"
    "./Channel.cc"
    "./Connector.cc"
    "./EventLoop.cc"
    "./EventLoopThread.cc"
    "./EventLoopThreadPool.cc"
    "./InetAddress.cc"
    "./Poller.cc"
    "./Socket.cc"
    "./SocketsOps.cc"
    "./TcpClient.cc"
    "./TcpConnection.cc"
    "./TcpSever.cc"
    "./Timer.cc"
    "./TimerQueue.cc"
)

add_library(netLib_net ${net_SRCS})
target_link_libraries(netLib_net netLib_base)

install(TARGETS netLib_net DESTINATION lib)

set(HEADERS
    "./Acceptor.h"
    "./Buffer.h"
    "./Callbacks.h"
    "./Channel.h"
    "./Connector.h"
    "./Endian.h"
    "./EventLoop.h"
    "./EventLoopThread.h"
    "./EventLoopThreadPool.h"
    "./InetAddress.h"
    "./Poller.h"
    "./Socket.h"
    "./SocketsOps.h"
    "./TcpClient.h"
    "./TcpConnection.h"
    "./TcpServer.h"
    "./Timer.h"
    "./TimerId.h"
    "./TimerQueue.h"
)

install(FILES ${HEADERS} DESTINATION include/netLib/net)

