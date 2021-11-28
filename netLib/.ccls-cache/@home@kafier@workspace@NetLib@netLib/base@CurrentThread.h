/*
 * @Date: 2021-11-27 21:33:28
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-27 21:33:31
 */
#pragma once
#include <stdint.h>

// extern的用法 当全局变量在定义初始化的时候， 通过使用extern 将作用域扩展到extern所在的作用域
namespace CurrentThread {
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;
void cacheTid();
inline int tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) {
    cacheTid();
  }
  return t_cachedTid;
}

inline const char* tidString()  // for logging
{
  return t_tidString;
}

inline int tidStringLength()  // for logging
{
  return t_tidStringLength;
}

inline const char* name() { return t_threadName; }
}
