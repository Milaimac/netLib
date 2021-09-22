#pragma once

#include <string>
#include "Types.h"
#include "Timestamp.h"

namespace netLib
{
namespace ProcessInfo{
    pid_t pid();

    string hostname();
}
} // namespace netLib
