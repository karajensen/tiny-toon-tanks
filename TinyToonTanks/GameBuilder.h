////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameBuilder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

class BulletPhysicsWorld;
struct SceneData;
struct GameData;

/**
* Builds all objects and diagnostics for the game
*/
class GameBuilder
{
public:

    GameBuilder();
    ~GameBuilder();

    /**
    * Initialises the game
    * @param scenedata All information for the scene
    * @param gamedata All information for the game
    * @param physics The physics engine
    * @return Whether the initialization was successful
    */
    bool Initialise(GameData& gamedata, 
                    SceneData& scenedata, 
                    BulletPhysicsWorld& physics);

private:

    /**
    * Prevent copying
    */
    GameBuilder(const GameBuilder&) = delete;
    GameBuilder& operator=(const GameBuilder&) = delete;

    /**
    * Initialises the game world
    * @param scenedata All information for the scene
    * @param gamedata All information for the game
    * @param physics The physics world
    * @return Whether the initialization was successful
    */
    bool InitialiseWorld(GameData& gamedata,
                         SceneData& scenedata, 
                         BulletPhysicsWorld& physics);

    /**
    * Initialises the controllable tanks
    * @param scenedata All information for the scene
    * @param gamedata All information for the game
    * @param physics The physics world
    * @return Whether the initialization was successful
    */
    bool InitialiseTanks(GameData& gamedata,
                         SceneData& scenedata, 
                         BulletPhysicsWorld& physics);

    /**
    * Initialises the tank bullets
    * @param scenedata All information for the scene
    * @param gamedata All information for the game
    * @param physics The physics world
    * @return Whether the initialization was successful
    */
    bool InitialiseBullets(GameData& gamedata,
                           SceneData& scenedata, 
                           BulletPhysicsWorld& physics);

private:

    int m_collisionGroupIndex = 0; ///< Current index for collision groups
};                     