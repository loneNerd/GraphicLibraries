#pragma once
#ifndef ENGINE_TOOLS_TIME_CLOCK_HPP_
#define ENGINE_TOOLS_TIME_CLOCK_HPP_

#include <chrono>

namespace Engine
{
namespace Tools
{
namespace Time
{
    class Clock
    {
    public:
        void Update()
        {
            m_lastTime = m_currentTime;
            m_currentTime = std::chrono::steady_clock::now();
            m_elapsed = m_currentTime - m_lastTime;

            if (m_isInit)
            {
                m_deltaTime = m_elapsed.count() > 0.1 ? 0.1f : static_cast<float>(m_elapsed.count());
                m_timeSinceStart += m_deltaTime * m_timeScale;
            }
            else
                Initialize();
        }

        float GetFramerate()         { return 1.0f / (m_deltaTime); }
        float GetDeltaTime()         { return m_deltaTime * m_timeScale; }
        float GetDeltaTimeUnscaled() { return m_deltaTime; }
        float GetTimeSinceStart()    { return m_timeSinceStart; }
        float GetTimeScale()         { return m_timeScale; }

        void Scale(float coeff) { m_timeScale *= coeff; }

        void SetTimeScale(float timeScale) { m_timeScale = timeScale; }

    private:
        void Initialize()
        {
            m_deltaTime = 0.0f;

            m_startTime = std::chrono::steady_clock::now();
            m_currentTime = m_startTime;
            m_lastTime = m_startTime;

            m_isInit = true;
        }

        std::chrono::steady_clock::time_point m_startTime;
        std::chrono::steady_clock::time_point m_lastTime;
        std::chrono::steady_clock::time_point m_currentTime;
        std::chrono::duration<double>         m_elapsed;

        bool  m_isInit         = false;
        float m_timeScale      = 1.0f;
        float m_deltaTime      = 0.0f;
        float m_timeSinceStart = 0.0f;
    };
}
}
}

#endif // ENGINE_TOOLS_TIME_CLOCK_HPP_
