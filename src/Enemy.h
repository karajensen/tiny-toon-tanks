////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Enemy.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tank.h"

#include "glm/matrix.hpp"

/**
* Enemy AI controlled tank
*/
class Enemy : public Tank
{
public:

    /**
    * Constructor
    * @param mesh Tankmesh Holds each piece of the tank
    * @param instance Which instance this tank is to access the mesh parts
    */
    Enemy(MeshGroup& tankmesh, int instance);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    virtual void AddToTweaker(Tweaker& tweaker) override;

    /**
    * Updates the tank
    */
    virtual void Update(float deltatime) override;

private:

    /**
    * Prevent copying
    */
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;

    /**
    * Creates movement for the AI to use
    */
    void CreateAIMovement();

private:

    float m_aiTimePassed = 0.0f; ///< Time passed to generate next ai mov
};