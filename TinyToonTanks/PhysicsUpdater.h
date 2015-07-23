////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsUpdater.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tank;
class BulletPhysicsWorld;
struct SceneData;
struct GameData;

/**
* Manager and owner of all objects
*/
class PhysicsUpdater
{
public:

    /**
    * Constructor
    * @param physics The physics world to update from
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    PhysicsUpdater(BulletPhysicsWorld& physics, 
                   GameData& gameData,
                   SceneData& sceneData);

    /**
    * Destructor
    */
    ~PhysicsUpdater();

    /**
    * Ticks the updater
    */
    void Tick();

private:

    /**
    * Prevent copying
    */
    PhysicsUpdater(const PhysicsUpdater&) = delete;
    PhysicsUpdater& operator=(const PhysicsUpdater&) = delete;

    /**
    * Updates the tank positions from the physics engine
    * @param tank The tank to update
    */
    void UpdateTankPositions(const Tank& tank);

    BulletPhysicsWorld& m_physics; ///< The physics world to update from
    GameData& m_gameData;          ///< Objects from the game to update
    SceneData& m_sceneData;        ///< Meshes from the scene to update
};
