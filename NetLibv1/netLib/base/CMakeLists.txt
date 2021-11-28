cmake_minimum_required(VERSION 3.10)

# 设置base 也是要顺序的。而不是单单安装头文件的字母顺序
set(base_SRCS
  AsyncLogging.cc
  CountDownLatch.cc
  CurrentThread.cc
  Date.cc
  FileUtil.cc
  LogFile.cc
  Logging.cc
  Logstream.cc
  ProcessInfo.cc
  Timestamp.cc
  Thread.cc
  TimeZone.cc
)

# Condition.cc 的WaitForSeconds直接被我放到了h文件中

add_library(netLib_base ${base_SRCS})
target_link_libraries(netLib_base pthread rt)

install(TARGETS netLib_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/netLib/base)

add_subdirectory(test)