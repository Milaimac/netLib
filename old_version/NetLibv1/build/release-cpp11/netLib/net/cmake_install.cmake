# Install script for directory: /home/kafier/workspace/NetLib/netLib/net

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/kafier/workspace/NetLib/build/release-cpp11/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/kafier/workspace/NetLib/build/release-cpp11/lib/libnetLib_net.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/netLib/net" TYPE FILE FILES
    "/home/kafier/workspace/NetLib/netLib/net/./Acceptor.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Buffer.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Callbacks.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Channel.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Connector.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Endian.h"
    "/home/kafier/workspace/NetLib/netLib/net/./EventLoop.h"
    "/home/kafier/workspace/NetLib/netLib/net/./EventLoopThread.h"
    "/home/kafier/workspace/NetLib/netLib/net/./EventLoopThreadPool.h"
    "/home/kafier/workspace/NetLib/netLib/net/./InetAddress.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Poller.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Socket.h"
    "/home/kafier/workspace/NetLib/netLib/net/./SocketsOps.h"
    "/home/kafier/workspace/NetLib/netLib/net/./TcpClient.h"
    "/home/kafier/workspace/NetLib/netLib/net/./TcpConnection.h"
    "/home/kafier/workspace/NetLib/netLib/net/./TcpServer.h"
    "/home/kafier/workspace/NetLib/netLib/net/./Timer.h"
    "/home/kafier/workspace/NetLib/netLib/net/./TimerId.h"
    "/home/kafier/workspace/NetLib/netLib/net/./TimerQueue.h"
    )
endif()

