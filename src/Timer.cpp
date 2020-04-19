////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"
#include "tweaker.h"
#include <Windows.h>

LARGE_INTEGER START = {};
LARGE_INTEGER CURRENT = {};
LARGE_INTEGER FREQUENCY = {};

Timer::Timer()
{
    QueryPerformanceFrequency(&FREQUENCY);
    QueryPerformanceCounter(&START);
}

void Timer::AddToTweaker(Tweaker& tweaker)
{
    tweaker.SetGroup("Timer");
    tweaker.AddEntry("Frames Per Second", &m_fps, TW_TYPE_INT32, true);
    tweaker.AddEntry("Delta Time", &m_deltaTime, TW_TYPE_FLOAT, true);
}

void Timer::UpdateTimer()
{
    m_previousElapsedMilliseconds = m_elapsedMilliseconds;
    QueryPerformanceCounter(&CURRENT);
    m_elapsedMilliseconds = (CURRENT.QuadPart - START.QuadPart) * 1000.0 / FREQUENCY.QuadPart;
    m_deltaTime = static_cast<float>(m_elapsedMilliseconds - m_previousElapsedMilliseconds);
    m_deltaTimeCounter += m_deltaTime;

    if (m_deltaTimeCounter >= 1000.0) //one second has passed
    {
        m_deltaTimeCounter = 0.0;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }
    else
    {
        ++m_fpsCounter; 
    }
}

float Timer::GetDeltaTime() const 
{ 
    return m_deltaTime;
}