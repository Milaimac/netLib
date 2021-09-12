#ifndef NETLIB_BASE_NONCOPYABLE
#define NETLIT_BASE_NONCOPYABLE

namespace netLib{
class noncopyable{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}

#endif