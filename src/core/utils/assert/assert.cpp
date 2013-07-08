#include "core/precomp/precomp.h"

namespace Acid
{
    void AssertHandler(const std::string& condition, const std::string& function, const std::string& file, unsigned int line)
    {
        ACID_LOG("=====================================================================");
        ACID_LOG("Assertion: %s", condition.c_str());
        ACID_LOG(" Function: %s", function.c_str());
        ACID_LOG("     File: %s", file.c_str());
        ACID_LOG("     Line: %d", line);
        ACID_LOG("=====================================================================");
    }
}
