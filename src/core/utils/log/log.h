#pragma once

#define ACID_LOG(format, ...)   Acid::Log(format, __VA_ARGS__)

namespace Acid
{
    void Log(const char* format, ...);
}
