# Install script for directory: /home/kafier/workspace/NetLib/netLib/base

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/kafier/workspace/NetLib/build/release-cpp11/lib/libnetLib_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/netLib/base" TYPE FILE FILES
    "/home/kafier/workspace/NetLib/netLib/base/AsyncLogging.h"
    "/home/kafier/workspace/NetLib/netLib/base/Atomic.h"
    "/home/kafier/workspace/NetLib/netLib/base/Condition.h"
    "/home/kafier/workspace/NetLib/netLib/base/CountDownLatch.h"
    "/home/kafier/workspace/NetLib/netLib/base/CurrentThread.h"
    "/home/kafier/workspace/NetLib/netLib/base/Date.h"
    "/home/kafier/workspace/NetLib/netLib/base/Exception.h"
    "/home/kafier/workspace/NetLib/netLib/base/FileUtil.h"
    "/home/kafier/workspace/NetLib/netLib/base/LogFile.h"
    "/home/kafier/workspace/NetLib/netLib/base/Logging.h"
    "/home/kafier/workspace/NetLib/netLib/base/Logstream.h"
    "/home/kafier/workspace/NetLib/netLib/base/Mutex.h"
    "/home/kafier/workspace/NetLib/netLib/base/ProcessInfo.h"
    "/home/kafier/workspace/NetLib/netLib/base/StringPiece.h"
    "/home/kafier/workspace/NetLib/netLib/base/Thread.h"
    "/home/kafier/workspace/NetLib/netLib/base/TimeZone.h"
    "/home/kafier/workspace/NetLib/netLib/base/Timestamp.h"
    "/home/kafier/workspace/NetLib/netLib/base/Types.h"
    "/home/kafier/workspace/NetLib/netLib/base/WeakCallback.h"
    "/home/kafier/workspace/NetLib/netLib/base/copyable.h"
    "/home/kafier/workspace/NetLib/netLib/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/kafier/workspace/NetLib/build/release-cpp11/netLib/base/test/cmake_install.cmake")

endif()

