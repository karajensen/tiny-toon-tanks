////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Bullet.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

class Mesh;

/**
* Object fired from a tank
*/
class Bullet
{
public:

    /**
    * Constructor
    * @param mesh The graphical mesh for a bullet
    * @param instance Which instance this bullet should update
    */
    Bullet(Mesh& mesh, int instance);

    /**
    * Sets the physics body ID
    */
    void SetPhysicsID(int ID);

    /**
    * @return the ID of the physics body
    */
    int GetPhysicsID() const;

    /**
    * @return whether this bullet is active in the world
    */
    bool IsActive() const;

    /**
    * Sets whether this bullet is active in the world
    */
    void SetActive(bool active);

    /**
    * Sets the world matrix of the bullet
    */
    void SetWorld(const glm::mat4& world);

private:

    /**
    * Prevent copying
    */
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;

    Mesh& m_mesh;         ///< The graphical mesh for a bullet
    int m_instance = 0;   ///< Which instance this bullet should update
    int m_physicsID = 0;  ///< The physics body ID
};