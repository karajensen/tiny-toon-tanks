////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tweaker;

/**
* Controllable tank
*/
class Tank
{
public:

    /**
    * Constructor
    */
    Tank();

    /**
    * Destructor
    */
    virtual ~Tank();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    virtual void AddToTweaker(Tweaker& tweaker);

private:

    /**
    * Prevent copying
    */
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;
};