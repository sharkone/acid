#pragma once

#include "core/timing/clock.h"

namespace Acid
{
    class Application
    {
    public:
        Application();
        
        int     Run();

    private:
        void    Update(float deltaTime);
        void    Render(float alpha);

        Clock   m_Clock;

        float   m_previousTime;
        float   m_accumulator;
    };
}
