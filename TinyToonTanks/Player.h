////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Player.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tank.h"

/**
* Player controlled tank
*/
class Player : public Tank
{
public:

    /**
    * Constructor
    */
    Player();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    virtual void AddToTweaker(Tweaker& tweaker) override;

private:

    /**
    * Prevent copying
    */
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
};