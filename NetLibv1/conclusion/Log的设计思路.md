- [Log的设计思路](#log的设计思路)
  - [Logging部分代码解析](#logging部分代码解析)
    - [LogLevel 显示了每次Log的等级](#loglevel-显示了每次log的等级)
    - [SourceFile](#sourcefile)
    - [buffer的主要阵地  Logstream](#buffer的主要阵地--logstream)
      - [FixedBuffer](#fixedbuffer)
    - [Timestamp 和 TimeZone](#timestamp-和-timezone)
      - [Timestamp的功能和实现](#timestamp的功能和实现)
    - [TimeZone又提供了什么](#timezone又提供了什么)
      - [TimeZone定义了哪些函数](#timezone定义了哪些函数)
    - [Date.h](#dateh)
  - [LogFile.h](#logfileh)
      - [先来看看FileUtil实现的功能](#先来看看fileutil实现的功能)
      - [那么LogFile实现的功能是](#那么logfile实现的功能是)
  - [最核心的模块 Asynclogging](#最核心的模块-asynclogging)
      - [append](#append)
      - [threadfunc](#threadfunc)

# Log的设计思路

- logging
  - 负责的是对外接口
  - 主要类 logger提供对外的接口
  - #define Log logger(\_\_FILE\__, \_\_LINE\_\_).stream();
  - 而本身的功能除了充当接口，还由设置Log的行、文件名还有level 等信息

- logstream
  - logstream 由一个数据成员buffer_。 所有的流操作都是操作Buffer这个对象作为缓冲区，这个 缓冲区的存在是为缓存成一行，将<<的结果连在一起
- LogFile 
  - LogFile封装了FileUtil，并设置一个循环数，过多少次就Flush一次
- asynclogging
  - 核心部分，负责启动一个log线程， 引用了"双缓冲技术"


## Logging部分代码解析
调用格式

```cpp
#define LOG Logger(__FILE__, __LINE__).stream()
LOG << "stream ok";
```
WebServer的取代方案（WebServer取代了什么)

去除部分
- SourceFile        使用filename代替，初值为"./WebServer.log"
- timestamp         去除掉
- TimeZone          去除掉
- LogLevel          去除掉         这个功能是最重要的，不可去除

新增部分
- phtread_once_t 
- AsynLogger

那么muduo的被去除模块的功能是什么？

### LogLevel 显示了每次Log的等级
```cpp
#define LOG_TRACE
 if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
  muduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()
```


1. 宏定义使用的Logger(Source file, int line, LogLevel level, const char* func) 其中level为trace ，第2个也是如此，level = debug
2. 第3个定义使用的Logger(Source file, int line)
3. 第4-7 定义使用的是Logger(Source file, int line, LogLevel level)
4. 第8-9定义使用的是Logger(Source file, int line, bool Abort) 当由错误码的时候，可以通过bool Abort判断。如果抛弃判断就直接输出LOG_FATAL 否则输出错误码


而实际上Logger的构造函数，传入的参数转交给Logstream 

~~~cpp
Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
  : time_(Timestamp::now()),
    stream_(),
    level_(level),
    line_(line),
    basename_(file)
{
  formatTime();
  CurrentThread::tid();
  stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
  stream_ << T(LogLevelName[level], 6);
  if (savedErrno != 0)
  {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

Logger::Logger(SourceFile file, int line)
  : impl_(INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, 0, file, line)
{
  impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : impl_(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort)
  : impl_(toAbort?FATAL:ERROR, errno, file, line)
{
}
~~~

### SourceFile 

相对直接传入filename

SourceFile 提供了将一个路径名 转化为文件名的操作。

~~~cpp

class SourceFile
{
public:
template<int N>
SourceFile(const char (&arr)[N])
    : data_(arr),
    size_(N-1)
{
    const char* slash = strrchr(data_, '/'); // builtin function
    if (slash)
    {
    data_ = slash + 1;
    size_ -= static_cast<int>(data_ - arr);
    }
}

explicit SourceFile(const char* filename)
    : data_(filename)
{
    const char* slash = strrchr(filename, '/');
    if (slash)
    {
    data_ = slash + 1;
    }
    size_ = static_cast<int>(strlen(data_));
}

const char* data_;
int size_;
};
~~~

### buffer的主要阵地  Logstream

基本功能一致

那么有哪些功能？

有一个FixedBuffer， 主要功能表现在append这个函数

#### FixedBuffer

FixedBuffer是Logstream的基础数据成员， 所有对Logstream的操作都转交给FixedBuffer

~~~cpp
void append(const char* buf, size_t len) {
    if (avail() > static_cast<int>(len)) {
        memcpy(cur_, buf, len);
        cur_ += len;
    }
}
typedef FixedBuffer<kSmallBuffer> Buffer;

~~~
### Timestamp 和 TimeZone

#### Timestamp的功能和实现

其中Timestamp和TimeZone实现的功能尤为复杂，其中在WebServer中，将它的操作移交给了Timer.h

而在muduo, Timestamp 实现的功能是

- 作为一个时间戳，设置定时时间
- 得到当前的时间戳，通过Timestamp::now()
- 以及其他的swap，add， operator==, operator<,timeDifference, invalid,valid
- 提供化为stirng的函数，如tostring(), toFormattedString

其中最重要的是tostirng,和toFormattedString还有Timestamp::now()

其中Timestamp::now()的实现

```cpp

Timestamp::Timestamp now(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds*kMicroSecondsPerSecond + tv.tv_sec);
}
```

注意两个使用的结构体以及两个函数
- 两个结构体
  - struct timeval tv 
  - struct tm tm_time
- 两个函数
  - **gmtime_r(&seconds, &tm_time)**
  - **gettimeofday(&tv, NULL)**
~~~cpp
struct tm tm_time;
gmtime_r(&second, &tm_time);
// 将从1970年1月1日 0：0：0到现在的秒数，转换到这个tm结构体，得到第一个epoch（1970）到现在有多少年，多少月，。。。
//如果要知道现在是多少年， 则+1970， 月+1；
~~~

---
~~~cpp
int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
// gettimeofday可以得到从第一个epcoh（1970年，1月1号)到现在的时间，并将之存储到timeval这个结构体中
通常使用模式
~~~

### TimeZone又提供了什么

~~~cpp
//TimeZone的构造函数
explicit TimeZone(const char* zonefile);
TimeZone (int eastOfutc, const char* tzname);
~~~

而TimeZone.cc又提供了**Transition结构体**， 用来初始化gmttime, localtime, localtimeIdx

- `gmttime` 格林威志时间 
- `localtime` 本地时间， 
- `localtimeIdx`

**而且还有comp这个结构体** 

Comp主要是实现了比较的功能
- 重载了operator() 实现比较
- equal成员函数

**Localtime结构体**
- `time_t gmtOffset`
- `bool isDst`
- `int arrbIdx`

**定义了TimeZone::data**


```cpp
struct TimeZone::Data{
    vector<detail::Transition> transitions;
    vector<detail::Localtime> localtimes;
    vector<string> names;
    string abbreviation;    //abberviation
};
```
由`localtime.tm_zone = &data.abberviation[local->arrbIdx]` 可以得知， 这里存储的是时区的简称


定义了File这个类对象，实现了readbyte(), readInt32, readUInt8
```cpp
readTimeZoneFile(const char* zonefile, struct TimeZone::Data* data)
```

![](https://pic.imgdb.cn/item/6146e3af2ab3f51d91976a0b.jpg)

#### TimeZone定义了哪些函数

1. readTimeZone ，估计是读取TimeZone对象，获取其中的时间信息
  其中的函数原型为bool readTineZoneFile(cosnt char* Zonefile, struct TimeZone::Data* data);

2. findLocaltime
   函数原型为 const LocalTime* findLocaltime(const TimeZone::Data& data, Transition& Entry, Comp comp)
   可以看出通过Entry查找data中的Localtime哪个是符合Entry要求的，并且将之返回


###  Date.h 

似乎是儒略日和公历的转换。 公式复杂不知道什么意思，没去研究

## LogFile.h

配套头文件 FileUtil.h

#### 先来看看FileUtil实现的功能

- expicit AppendFile(const::string filename)
- append(const char* logline, int len)
- flush()
- write()
  
--- 

- AppendFile实现内容是将filename 对应的文件流打开 fp_

- append(const char* logline, int len) 将对应日志行通过调用write函数，将数据写入文件， 并且负责异常处理，以及保证完整地写入文件

- flush() 让文件fflush

- write(const string * logline, size_t len)   来调用fwrite_unlocked(logline, 1, sizeof(logline), fp_)，将日志行写入日志文件

---

#### 那么LogFile实现的功能是
- 文件的回滚（根据时间、文件大小进行回滚归档）
- 主要核心功能依然在append

只不过这里的append是有加锁和没加锁版本
![](https://pic.imgdb.cn/item/614b3d472ab3f51d9102a5ef.jpg)

并且append_unlocked还需要实现文件的回滚rollFile

```cpp
void LogFile::append_unlocked(const char* logline, int len){
    file_->append(logline, len);

    if(file_->writtenBytes() > rollSize_){
        rollFile();
    }
    else{// writtenBytes <=  rollSize 没有到达规定的文件大小，但是达到规定的时间， 这里规定的是24小时。 这样的话，下面的又乘又除倒是可以理解了
        ++count_;
        if(count_ >= checkEveryN_){// 只不过这里的检查是根据checkEveryN的
            count_ = 0;
            time_t now = ::time(NULL);
            time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
            if(thisPeriod_ != startOfperiod_){      
                rollFile();
            }
            else if(now - lastFlush_ > flushInterval_){
                lastFlush_ = now;
                file_->flush();
            }
        }
    }
}
```

顺带复习一下unique_ptr， unique_ptr的所有权，只能转移，不能复制

~~~cpp
std::unique_ptr<Task> taskPtr5(new Task(55));
// 不为空
if(taskPtr5 != nullptr)
	std::cout<<"taskPtr5 is not empty"<<std::endl;
// 释放关联指针的所有权
Task * ptr = taskPtr5.release();
// 现在为空
if(taskPtr5 == nullptr)
	std::cout<<"taskPtr5 is empty"<<std::endl;
~~~


## 最核心的模块 Asynclogging

Asynclogging通过4个buffer实现的

核心思想
- 前端负责向BufferA填写数据（日志信息）， 后端将BufferB写入文件（logfile）
- 当BufferA写满了，交换A和B，让后端将BufferA写入文件

#### append

先看看核心的append

- 如果现在的buffer还有余地时， currentBuffer_->append(logline, len)
- 否则buffers_.push_back(std::move(currentBuffer_))， 将现在的buffer放回buffers  （这里就是BufferA写满的情况）
- 查看下一个buffer - nextBuffer_
- 如果有 currentBuffer_ = std::move(nextBuffer_)
- 否则 currentBuffer_.reset(new Buffer)
- 最终 currentBuffer_.append(logline, len)
~~~cpp
void AsyncLogging::append(const char* logline, int len)
{
  muduo::MutexLockGuard lock(mutex_);
  if (currentBuffer_->avail() > len)
  {
    currentBuffer_->append(logline, len);
  }
  else
  {
    buffers_.push_back(std::move(currentBuffer_));

    if (nextBuffer_)
    {
      currentBuffer_ = std::move(nextBuffer_);
    }
    else
    {
      currentBuffer_.reset(new Buffer); // Rarely happens ，如果说nextBuffer还没有创建，那么就需要直接给现在的buffer reset
    }
    currentBuffer_->append(logline, len);
    cond_.notify();
  }
}
~~~


#### threadfunc

1. 先创建logFile对象
2. 创建两个buffer； newBuffer1， newBuffer2
   - 并且进行初始化 newBuffer1->bzero(), newBuffer2->bzero(), 
3. 创建BufferVector对象 `BufferVector BuffertoWrite`
4. 运行时 `while (running_)`
   1. 加锁 `MutexLockGuard lock(mutex_)`
   2. 如果buffers为空， 则采取等待`cond_.watiForSeconds(flushInterval)`
   3. 将现在的buffer放入buffers数组中， `buffers.push_back(std::move(currentBuffer_))`
   4. 将newBuffer1放入currentBuffer_, `currentBuffer_=std::move(newBuffer1)`
   5. `buffersToWrite.swap(buffers_)`， bufferToWrite获得现有的buffers数组，准备写入
   6. 如果nextBuffer_为空， 则 `nextBuffer_ = std::move(newBuffer2)`
5. 如果bufferToWrite过多的时候， 则输送错误信息，并删去过多信息，留下[bufferToWrite.begin()+2 ,bufferToWrite.end()] 
6. 将bufferToWrite的数据放入output (logfile) , for(auto buffer:bufferToWrite)   output.append(buffer, sizeof buffer)
7. 


```cpp

void netLib::threadFunc(){

}

```