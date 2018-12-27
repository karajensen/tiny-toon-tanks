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
* Controls the spawning and movement of bullets
*/
class BulletManager
{
public:

    /**
    * Constructor
    * @param physics The physics world
    * @param gameData Objects from the game to update
    * @param sceneData Meshes from the scene to update
    */
	BulletManager(PhysicsEngine& physics,
                  GameData& gameData,
                  SceneData& sceneData);

    /**
    * Destructor
    */
    ~BulletManager();

    /**
    * Ticks the updater before physics have updated
    */
    void PrePhysicsTick();

	/**
	* Ticks the updater after physics has been updated
	*/
	void PostPhysicsTick();

private:

    /**
    * Prevent copying
    */
	BulletManager(const BulletManager&) = delete;
	BulletManager& operator=(const BulletManager&) = delete;

    /**
    * Fires a bullet from the tank if requested
    */
    void FireBullet(const Tank& tank);

    /**
    * Moves the bullet
    */
    void MoveBullet(Bullet& bullet);

	/**
	* Updates the bullet position from the physics engine
	*/
	void UpdateButtonPosition(Bullet& bullet);

    PhysicsEngine& m_physics;  ///< The physics world to update from
    GameData& m_gameData;      ///< Objects from the game to update
    SceneData& m_sceneData;    ///< Meshes from the scene to update
};
