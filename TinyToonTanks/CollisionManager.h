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


private:

    /**
    * Prevent copying
    */
    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    PhysicsEngine& m_physics;             ///< The physics world to update from
    GameData& m_gameData;                 ///< Objects from the game to update
    SceneData& m_sceneData;               ///< Meshes from the scene to update
    std::vector<CollisionEvent> m_events; ///< List of currently occuring collision events
};
