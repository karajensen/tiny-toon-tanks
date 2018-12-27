////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TankMovementUpdater.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tank;
class PhysicsEngine;
struct SceneData;
struct GameData;

/**
* Controls the movement of the tanks
*/
class TankMovementUpdater
{
public:

    /**
    * Constructor
    * @param physics The physics world to update from
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    TankMovementUpdater(PhysicsEngine& physics,
                        GameData& gameData,
                        SceneData& sceneData);

    /**
    * Destructor
    */
    ~TankMovementUpdater();

    /**
    * Ticks the updater before physics have updated
    * @param deltatime The time between ticks in seconds
    */
    void PrePhysicsTick(float deltatime);

    /**
    * Ticks the updater after physics have updated
    */
    void PostPhysicsTick();

private:

    /**
    * Prevent copying
    */
    TankMovementUpdater(const TankMovementUpdater&) = delete;
    TankMovementUpdater& operator=(const TankMovementUpdater&) = delete;

    /**
    * Updates the tank positions from the physics engine
    * @param tank The tank to update
    */
    void UpdateTankPositions(const Tank& tank);

    /**
    * Manipulates the tank from its movement requests
    * @param deltatime The time between ticks in seconds
    * @param tank The tank to update
    */
    void UpdateTankMovement(float deltatime, Tank& tank);

    /**
    * Manipulates the gun from its movement requests
    * @param deltatime The time between ticks in seconds
    * @param tank The tank to update
    */
    void UpdateGunMovement(float deltatime, Tank& tank);

    /**
    * Flips the tank if requested
    * @param tank The tank to update
    */
    void FlipTank(const Tank& tank);

    PhysicsEngine& m_physics;   ///< The physics world to update from
    GameData& m_gameData;       ///< Objects from the game to update
    SceneData& m_sceneData;     ///< Meshes from the scene to update
};
