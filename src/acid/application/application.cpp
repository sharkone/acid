#include "acid/precomp/precomp.h"

#include "acid/application/application.h"

#define UPDATE_RATE             60.0f
#define UPDATE_DELTATIME        (1.0f / UPDATE_RATE)
#define UPDATE_MAX_FRAMETIME    0.25f

namespace Acid
{
    Application::Application()
    {
        m_previousTime = m_Clock.GetElapsedTime();
        m_accumulator  = 0.0f;
    }

    int Application::Run()
    {
        while (true)
        {
            m_Clock.Update();

            float newTime   = m_Clock.GetElapsedTime();
            float frameTime = Maths::Min(newTime - m_previousTime, 0.25f);

            m_accumulator += frameTime;

            while (m_accumulator >= UPDATE_DELTATIME)
            {
                Update(UPDATE_DELTATIME);
                m_accumulator -= UPDATE_DELTATIME;
            }

            float alpha = m_accumulator / UPDATE_DELTATIME;
            Render(alpha);

            m_previousTime = newTime;
        }

        return (EXIT_SUCCESS);
    }

    void Application::Update(float deltaTime)
    {
    }

    void Application::Render(float alpha)
    {
    }
}
