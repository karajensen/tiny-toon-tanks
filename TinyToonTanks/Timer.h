////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tweaker;

/**
* FPS class for determining frame rate and delta time
*/
class Timer
{
public:

    /**
    * Constructor
    */
    Timer();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Updates the timer to determine delta-time and fps
    */
    void UpdateTimer();

    /**
    * @return The time passed since last frame in seconds
    */
    float GetDeltaTime() const;

private:
    float m_deltaTime = 0.0f;
    long double m_elapsedMilliseconds = 0.0;
    long double m_previousElapsedMilliseconds = 0.0;
    unsigned int m_fps = 0;
    unsigned int m_fpsCounter = 0;
    float m_deltaTimeCounter = 0.0f;
};

