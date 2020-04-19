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
    * @param mesh Tankmesh Holds each piece of the tank
    * @param instance Which instance this tank is to access the mesh parts
    */
    Player(MeshGroup& tankmesh, int instance);

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