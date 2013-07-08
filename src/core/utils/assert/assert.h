#pragma once

#define __ACID_STRINGIFY(s) #s
#define ACID_STRINGIFY(s)   __ACID_STRINGIFY(s)

#define ACID_BREAK()    \
    __debugbreak()

#define ACID_ASSERT(condition)                                                                          \
    do                                                                                                  \
    {                                                                                                   \
        if ((condition) == false)                                                                       \
        {                                                                                               \
            Acid::AssertHandler(ACID_STRINGIFY(condition), BOOST_CURRENT_FUNCTION, __FILE__, __LINE__); \
            ACID_BREAK();                                                                               \
        }                                                                                               \
    } while (0)

namespace Acid
{
    void AssertHandler(const std::string& condition, const std::string& function, const std::string& file, unsigned int line);
}
