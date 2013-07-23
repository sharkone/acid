#pragma once

namespace Acid
{
    class Clock
    {
    public:
        Clock();

        void	Update();

        float	GetElapsedTime() const;

    private:
        __int64	m_Frequency;
        __int64	m_Start;

        float	m_ElapsedTime;
    };
}