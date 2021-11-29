#include "ProcessInfo.h"

namespace netLib{

pid_t ProcessInfo::pid(){
    return ::getpid();
}
string ProcessInfo::hostname(){
    char buf[256];
    if(::gethostname(buf, sizeof buf) == 0){
        buf[sizeof(buf)-1] = '\0';
        return buf; 
    }else{
        return "unknownhost";
    }
}


}