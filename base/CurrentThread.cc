#include"netLib/base/CurrentThread.h"
#include<execinfo.h>
#include<cxxabi.h>
namespace netLib{
namespace CurrentThread{
    // set init value
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* threadName = "unknown";
    // static_assert 静态断言， 用来做编译器的断言
    // std::is_same<typename uname, typename uname> 判断两个typename类型是否一致
    static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");
    string stackTrace(bool demangle){
        string stack;
        const int max_frames = 200;
        void* frame[max_frames];
        int nptrs = ::backtrace(frame, max_frames);
        char** strings = ::backtrace_symbols(frame, nptrs);
        if(strings){
            size_t len = 256;
            char* demangled = demangle ? static_cast<char*> (malloc(len)) : nullptr;
            for(int i = 1;i < nptrs;++i){ //skipping the 0-th, which is this function
                if(demangle){
                    char* left_par = nullptr;
                    char* plus = nullptr;
                    // https://panthema.net/2008/0901-stacktrace-demangled/ 
                    for(char* p = strings[i];*p;++p){
                        if(*p == '('){
                            left_par = p;
                        }
                        else if(*p == '+'){
                            plus = p;
                        }
                    }

                    if(left_par && plus){
                        *plus = '\0';
                        int status = 0;
                        char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);
                        *plus = '+';
                        if(status == 0){
                            demangled = ret;
                            stack.append(strings[i], left_par+1);   //从头地址到'('
                            stack.append(demangled);                //将demangled放入stack
                            stack.append(plus);                     //在将之前的plus放入
                            stack.push_back('\n');                  //还原成demangled后的C++标识符
                            continue;
                        }
                    }
                }
                stack.append(strings[i]);
                stack.push_back('\n');
            }
            free(demangled);
            free(strings);
        }
        return stack;
    }

}
}

// int backtrace(void **buffer, int size ) 该函数将用来获取程序中当前函数的回溯信息，即一系列的函数调用关系，获取到的信息放到buffer
// buffer是一个数组指针，数组的每一个元素保存着每一级调用函数的返回地址
// 参数size指定列buffer中可存放的返回地址的数量， 如果size < 实际的回溯层级，那么buffer中只能存放最近size条的函数调用关系

// char** backtrace_symbols(void* const buffer, int size)  可以将返回地址都对应到函数名，参数size是buffer中的条目数
// backtrace_symbols可以把每一个返回值都翻译成“函数名+函数偏移值+函数返回值”

//void backtrace_symbols_fd(void *const *buffer, int size, int fd);
//它翻译的函数回溯信息不是放到返回值中， 而是一行一行地放入fd对应的文件