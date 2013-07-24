#pragma once

namespace Acid
{
    class Maths
    {
    public:
        template <typename T>
        static T   Min(const T& value1, const T& value2)
        {
            return (std::min(value1, value2));
        }

        template <typename T>
        static T   Max(const T& value1, const T& value2)
        {
            return (std::max(value1, value2));
        }

        template <typename T>
        static T   Clamp(const T& value, const T& min, const T& max)
        {
            return (Max(Min(value, max), min));
        }
    };
}
