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
    * Sets whether to request a forward impulse on this bullet
    */
    void SetNeedsImpulse(bool needsImpulse);

    /**
    * @return Whether to request a forward impulse on this bullet
    */
    bool NeedsImpulse();

    /**
    * Whether the tank has just fired this bullet in a set duration
    */
    void SetJustShot(bool justShot);

    /**
    * @return whether the tank has just fired this bullet in a set duration
    */
    bool JustShot() const;

    /**
    * Whether this bullet can contribute to scoring
    */
    void SetAllowScore(bool allowScore);

    /**
    * @return Whether this bullet can contribute to scoring
    */
    bool AllowScore() const;

    /**
    * @return the position of the bullet
    */
    const glm::vec3& Position() const;

    /**
    * @return the forward vector of the bullet
    */
    glm::vec3 Forward() const;

    /**
    * Reduces the life of the bullet
    */
    void TakeDamage(int amount);

    /**
    * @return the amount of damage this bullet can do
    */
    int DamageDealt() const;

    /**
    * @return the amount of health this bullet has
    */
    int Health() const;

    /**
    * @return whether the counter to determine when the bullet can be rendered is active
    */
    bool UsingDrawCounter() const;

    /**
    * Updates the draw counter to determine when the bullet can be rendered
    */
    void UpdateDrawCounter();

private:

    /**
    * Prevent copying
    */
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;

    Mesh& m_mesh;                    ///< The graphical mesh for a bullet
    bool m_alive = false;            ///< Whether this bullet is considered alive in the world
    int m_instance = 0;              ///< Which instance this bullet should update
    int m_physicsID = 0;             ///< The physics body ID
    int m_health = 0;                ///< The health of the bullet before it is considered dead
    bool m_justShot = false;         ///< Set to false after a set duration of the tank shooting
    bool m_allowScore = false;       ///< Whether this bullet can contribute to scoring
    bool m_needsImpulse = false;     ///< Whether to request a forward impulse on this bullet
    int m_impulseCounter = 0;        ///< Counter for generating the forward impulse
    bool m_usingDrawCounter = false; ///< Whether to request enabling visibility on this bullet
    int m_drawCounter = 0;           ///< Counter of requesting visibility
};