#include "core/precomp/precomp.h"

#include "core/timing/clock.h"

namespace Acid
{
    Clock::Clock()
        :	m_ElapsedTime(0.0f)
    {
        {
            BOOL result = QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_Frequency));
            ACID_ASSERT(result == TRUE);
        }

        {
            BOOL result = QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_Start));
            ACID_ASSERT(result == TRUE);
        }
    }

    void Clock::Update()
    {
        __int64 timeStamp;
        BOOL result = QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&timeStamp));
        ACID_ASSERT(result == TRUE);

        m_ElapsedTime = static_cast<float>(timeStamp - m_Start) / static_cast<float>(m_Frequency);
    }

    float Clock::GetElapsedTime() const
    {
        return (m_ElapsedTime);
    }
}
