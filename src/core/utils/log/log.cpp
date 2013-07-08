#include "core/precomp/precomp.h"

namespace Acid
{
    void Log(const char* format, ...)
    {
        std::va_list args;
        va_start(args, format);
        std::vprintf(format, args);
        std::printf("\n");
    }
}
