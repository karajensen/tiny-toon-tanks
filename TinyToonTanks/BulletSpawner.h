////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletSpawner.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Tank;
class Bullet;
class PhysicsEngine;
struct SceneData;
struct GameData;

/**
* Controls the spawning of bullets
*/
class BulletSpawner
{
public:

    /**
    * Constructor
    * @param physics The physics world
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
    BulletSpawner(PhysicsEngine& physics, 
                  GameData& gameData,
                  SceneData& sceneData);

    /**
    * Destructor
    */
    ~BulletSpawner();

    /**
    * Ticks the updater
    * @param deltatime The time between ticks in seconds
    */
    void Tick(float deltatime);

private:

    /**
    * Prevent copying
    */
    BulletSpawner(const BulletSpawner&) = delete;
    BulletSpawner& operator=(const BulletSpawner&) = delete;

    /**
    * Fires a bullet from the tank if requested
    */
    void FireBullet(const Tank& tank);

    /**
    * Moves the bullet
    */
    void MoveBullet(Bullet& bullet);

    PhysicsEngine& m_physics;  ///< The physics world to update from
    GameData& m_gameData;      ///< Objects from the game to update
    SceneData& m_sceneData;    ///< Meshes from the scene to update
};
