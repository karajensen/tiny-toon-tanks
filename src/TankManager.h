////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TankManager.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tank;
class PhysicsEngine;
struct SceneData;
struct GameData;

/**
* Controls the movement of the tanks
*/
class TankManager
{
public:

    /**
    * Constructor
    * @param physics The physics world to update from
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    TankManager(PhysicsEngine& physics,
                GameData& gameData,
                SceneData& sceneData);

    /**
    * Destructor
    */
    ~TankManager();

    /**
    * Ticks the updater before physics have updated
    */
    void PrePhysicsTick(float physicsDeltaTime);

    /**
    * Ticks the updater after physics have updated
    */
    void PostPhysicsTick();

private:

    /**
    * Prevent copying
    */
    TankManager(const TankManager&) = delete;
    TankManager& operator=(const TankManager&) = delete;

    /**
    * Updates the tank positions from the physics engine
    * @param tank The tank to update
    */
    void UpdateTankPositions(const Tank& tank);

    /**
    * Manipulates the tank from its movement requests
    */
    void UpdateTankMovement(float physicsDeltaTime, Tank& tank);

    /**
    * Manipulates the gun from its movement requests
    */
    void UpdateGunMovement(float physicsDeltaTime, Tank& tank);

    PhysicsEngine& m_physics;    ///< The physics world to update from
    GameData& m_gameData;        ///< Objects from the game to update
    SceneData& m_sceneData;      ///< Meshes from the scene to update
};
