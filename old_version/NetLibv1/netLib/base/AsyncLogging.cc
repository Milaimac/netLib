#include "AsyncLogging.h"
#include "LogFile.h"
#include "Timestamp.h"

#include <stdio.h>
using namespace netLib;

AsyncLogging::AsyncLogging(const std::string &basename,
                           off_t rollsize,
                           int flushInterval)
  : flushInterval_(flushInterval),
    running_(false),
    basename_(basename),
    rollSize_(rollsize),
    thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
    latch_(1),
    mutex_(),
    cond_(mutex_),
    currentbuffer_(new Buffer),
    nextbuffer_(new Buffer),
    buffers_()
{
    currentbuffer_->bzero();
    nextbuffer_->bzero();
    buffers_.reserve(16); //预留16个buffer
}

void AsyncLogging::append(const char *logline, int len)
{
    netLib::MutexLockGuard lock(mutex_);
    if (currentbuffer_->avail() > len)
    {
        currentbuffer_->append(logline, len);
    }
    else
    {
        buffers_.push_back(std::move(currentbuffer_));
        if (nextbuffer_)
        {
            currentbuffer_ = std::move(nextbuffer_);
        }
        else
        {
            currentbuffer_.reset(new Buffer);
        }
        currentbuffer_->append(logline, len);
        cond_.notify();
    }
}

void AsyncLogging::threadFunc()
{
    assert(running_ == true);
    latch_.countDown();
    LogFile output(basename_, rollSize_, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();

    BufferVector bufferToWrite;
    bufferToWrite.reserve(16);

    while (running_)
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(bufferToWrite.empty());

        {
            MutexLockGuard lock(mutex_);
            if (buffers_.empty())
            { //如果buffers_.empty()就说明currentBuffer没有放入buffers_,也就是前端写入数据极少
                cond_.waitForSeconds(flushInterval_);
            }
            buffers_.push_back(std::move(currentbuffer_)); //等待之后将当前buffer放入交付给后端
            currentbuffer_ = std::move(newBuffer1);         // add 
            bufferToWrite.swap(buffers_);
            if (!nextbuffer_)
            { //如果nextBuffer已经传给了currentBuffer
                nextbuffer_ = std::move(newBuffer2);
            }
        }

        assert(!bufferToWrite.empty());

        if (bufferToWrite.size() > 25)
        { // 日志数据过多，buffer本来reserve16个的，现在是25个， 直接把bufferToWrite.begin()+2, 到end的全部删掉
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log message at %s, %zd lager buffer\n",
                     Timestamp::now().toFormattedString().c_str(),
                     bufferToWrite.size() - 2); 
            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            bufferToWrite.erase(bufferToWrite.begin() + 2, bufferToWrite.end());
        }

        for (const auto &buffer : bufferToWrite)
        {
            output.append(buffer->data(), buffer->length());
        }

        if (bufferToWrite.size() > 2)
        {
            bufferToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!bufferToWrite.empty());
            newBuffer1 = std::move(bufferToWrite.back());
            bufferToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!bufferToWrite.empty());
            newBuffer2 = std::move(bufferToWrite.back());
            bufferToWrite.pop_back();
            newBuffer2->reset();
        }
    // 忘记给写缓冲区进行clear()
    // 并且没有给output.flush()
        bufferToWrite.clear();
        output.flush();
    }
    output.flush();
}