#pragma once
#include <assert.h>
#include <string.h>
#include <string>
#include "noncopyable.h"
namespace netLib{

const int KSmallBuffer = 4000;
const int KLargeBuffer = 4000*1000;

template<int SIZE> //通过这样限定SIZE 为一个整型
class FixedBuffer:noncopyable{
public:
    FixedBuffer():cur_(data_){
        //TODO
    }

    ~FixedBuffer(){
        // TODO
    }
    
    void append(const char* buf, size_t len){
        if(avail() > static_cast<int>(len)){
            memcpy(data_, buf, len);
            cur_ += len;
        }
    }
    int length(){
        return static_cast<int> (cur_ - data_);
    } 

    int avail(){
        return end() - cur_;
    }

    // const char* end(){
    //     return  data_ +  sizeof data_;
    // }
    // it should be private

    char* current(){
        return cur_;
    }
    void add(size_t len){
        return cur_ + len;
    }
    void bzero(){
        memset(data_, 0, sizeof data_);
    }
    void reset(){
        cur_ = data_;
    }


private:
    const char* end() const{    return data_ + sizeof data_;}
    // void* (cookie_)();
    char data_[SIZE];           
    char* cur_;

};

class LogStream:noncopyable{
public:
    typedef FixedBuffer<KSmallBuffer> Buffer;

    LogStream& operator<< (bool v){
        buffer_.append(v ? "1" : "0" , 1);
        return *this;
    }

    LogStream& operator<< (short);
    LogStream& operator<< (unsigned short);
    LogStream& operator<< (int);
    LogStream& operator<< (unsigned int);
    LogStream& operator<< (long);
    LogStream& operator<< (unsigned long);
    LogStream& operator<< (long long);
    LogStream& operator<< (unsigned long long);

    LogStream& operator<< (const void*);

    LogStream& operator<< (float v){
        *this << static_cast<double>(v);
        return *this;
    }

    LogStream& operator<< (double);
    LogStream& operator<< (long double);

    LogStream& operator<< (char v){
        buffer_.append(&v, 1);
        return *this;
    }

    LogStream& operator<< (const char* str){
        if(str){
            buffer_.append(str, sizeof str); //FIXME strlen()
        }
        else{
            buffer_.append("(null)", 6);
        }
        return *this;
    }

    LogStream& operator<< (const std::string& v){
        buffer_.append(v.c_str(), v.size());
        return  *this;
    }   

    void append(const char* data, int len){
        buffer_.append(data, len);
    }

    const Buffer& buffer() const {return buffer_;}

    void resetBuffer(){buffer_.reset();}
private:
    void staticCheck();

    template<typename T>
    void formatInteger(T);

    Buffer buffer_;

    static const int kMaxNumericSize = 32;
};


}