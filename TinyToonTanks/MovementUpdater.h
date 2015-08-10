////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - MovementUpdater.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tank;
class PhysicsEngine;
struct SceneData;
struct GameData;

/**
* Controls the movement of the tanks
*/
class MovementUpdater
{
public:

    /**
    * Constructor
    * @param physics The physics world to update from
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    MovementUpdater(PhysicsEngine& physics, 
                    GameData& gameData,
                    SceneData& sceneData);

    /**
    * Destructor
    */
    ~MovementUpdater();

    /**
    * Ticks the updater
    * @param deltatime The time between ticks in seconds
    */
    void Tick(float deltatime);

private:

    /**
    * Prevent copying
    */
    MovementUpdater(const MovementUpdater&) = delete;
    MovementUpdater& operator=(const MovementUpdater&) = delete;

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
