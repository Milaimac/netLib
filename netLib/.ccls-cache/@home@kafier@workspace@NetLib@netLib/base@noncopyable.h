/*
 * @Date: 2021-11-27 21:29:06
 * @LastEditors: kafier
 * @LastEditTime: 2021-11-27 21:29:07
 */
#pragma once

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable &);
    const noncopyable &operator=(const noncopyable &);
};