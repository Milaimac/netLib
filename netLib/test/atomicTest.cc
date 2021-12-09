/*
 * @Date: 2021-12-06 14:11:00
 * @LastEditors: kafier
 * @LastEditTime: 2021-12-06 14:14:21
 */
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <sstream>

std::atomic_flag lock = ATOMIC_FLAG_INIT;
std::stringstream stream;

void append_number(int x)
{
    while(lock.test_and_set());     //如果被设置了， 那么lock.test_and_set会死循环。 那么就无法执行下面的序列
    stream << "thread#" << x << "\n";
    lock.clear();
}

int main()
{
    std::vector<std::thread> ths;
    for(int i = 0;i < 10;++i)
    {
        ths.push_back(std::thread(append_number, i));
    }

    for(int i =0;i<10;++i)
    {
        ths[i].join();
    }
    std:: cout << stream.str();
    return 0;
}