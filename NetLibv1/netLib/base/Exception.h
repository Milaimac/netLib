#pragma once    

#include "../base/Types.h"
#include <exception>

namespace netLib
{
    class Exception: public std::exception
    {
    public:
        Exception(string what);

        ~Exception() noexcept override = default;

        const char* what() const noexcept override
        {
            return message_.c_str();
        }

        const char* stackTrace() const noexcept
        {
            return stack_.c_str();
        }
    private:
        string message_;
        string stack_;
    };
}