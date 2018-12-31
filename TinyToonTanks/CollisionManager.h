////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionManager.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CollisionEvent.h"

#include <vector>

class Tank;
class PhysicsEngine;
struct SceneData;
struct GameData;

/**
* Managers detection and resolve of collisions
*/
class CollisionManager
{
public:

    /**
    * Constructor
    * @param physics The physics world to update from
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    CollisionManager(PhysicsEngine& physics,
                     GameData& gameData,
                     SceneData& sceneData);

    /**
    * Destructor
    */
    ~CollisionManager() = default;

    /**
    * Detection of which objects collide with what
    */
    void CollisionDetection();

    /**
    * Determine fate of collided objects
    */
    void CollisionResolution();

    /**
    * Returns the collision group index
    */
    int GetCollisionGroupIndex() const;

    /**
    * Increments the collision group index
    */
    void IncrementCollisionGroupIndex();

private:

    /**
    * Prevent copying
    */
    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    /**
    * Determines what to do for a specific collision event
    */
    void ResolveCollisionEvent(CollisionEvent* colEvent);

    /**
    * Obtain a tank depending on the ID
    */
    Tank* GetTank(int instanceID) const;

    /**
    * Determines what to do when a bullet interacts with another shape
    */
    void BulletCollisionLogic(const CollisionEvent* collisionEvent);

    /**
    * Determines what to do when a tank interacts with another shape
    */
    void TankCollisionLogic(const CollisionEvent* collisionEvent);

    PhysicsEngine& m_physics;             ///< The physics world to update from
    GameData& m_gameData;                 ///< Objects from the game to update
    SceneData& m_sceneData;               ///< Meshes from the scene to update
    std::vector<CollisionEvent> m_events; ///< List of currently occuring collision events
    int m_collisionGroupIndex = 0;        ///< Current index for collision groups
};
