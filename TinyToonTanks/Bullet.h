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
    * Sets the physics body ID that shot the bullet
    */
    void SetOwnerID(int ID);

    /**
    * @return the ID of the physics body that shot the bullet
    */
    int GetOwnerID() const;

    /**
    * @return whether this bullet is alive in the world
    */
    bool IsAlive() const;

    /**
    * Sets whether this bullet is alive in the world
    */
    void SetIsAlive(bool alive);

    /**
    * Resets the bullet
    */
    void Reset();

    /**
    * Sets the world matrix of the bullet
    */
    void SetWorld(const glm::mat4& world);

    /**
    * @return the position of the bullet
    */
    const glm::vec3& GetPosition() const;

    /**
    * Reduces the life of the bullet
    */
    void TakeDamage(int amount);

    /**
    * @return the amount of health this bullet has
    */
    int Health() const;

    /**
    * Sets whether to generate a quick impulse or continuous movement
    */
    void SetGenerateImpulse(bool generate, const glm::vec3& direction);

    /**
    * Gets whether to generate a quick impulse or continuous movement
    */
    bool ShouldGenerateImpulse() const;

    /**
    * @return the direction to generate the impulse from
    */
    const glm::vec3& GetGeneratedImpulseDirection() const;

    /**
    * @return the position the bullet was initially fired from
    */
    const glm::vec3& GetFiredPosition() const;

    /**
    * Set the position the bullet was initially fired from
    */
    void SetFiredPosition(const glm::vec3& position);

private:

    /**
    * Prevent copying
    */
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;

    Mesh& m_mesh;                         ///< The graphical mesh for a bullet
    glm::vec3 m_firePosition;             ///< Position the bullet was initially fired from
    glm::vec3 m_generateImpulseDirection; ///< Direction to generate the impulse from
    bool m_alive = false;                 ///< Whether this bullet is considered alive in the world
    int m_instance = 0;                   ///< Which instance this bullet should update
    int m_physicsID = 0;                  ///< The physics body ID
    int m_ownerID = 0;                    ///< Physics body ID of the owner of the bullet
    int m_health = 0;                     ///< The health of the bullet before it is considered dead
    bool m_generateImpuse = false;        ///< Whether to generate a quick impulse or continuous movement
};