////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Enemy.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tank.h"

/**
* Enemy AI controlled tank
*/
class Enemy : public Tank
{
public:

    /**
    * Constructor
    */
    Enemy();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    virtual void AddToTweaker(Tweaker& tweaker) override;

private:

    /**
    * Prevent copying
    */
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
};