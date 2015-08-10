////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Game.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Postprocessing.h"
#include "glm/glm.hpp"

class MovementUpdater;
class BulletSpawner;
class GameBuilder;
class PhysicsEngine;
class Tweaker;
class Camera;
struct SceneData;
struct GameData;

/**
* Manager of the playable game
*/
class Game
{
public:

    /**
    * Constructor
    * @param camera The main view camera
    */
    Game(Camera& camera);

    /**
    * Destructor
    */
    ~Game();

    /**
    * Ticks the game
    * @param deltatime The time passed between ticks
    */
    void Tick(float deltatime);

    /**
    * Initialises the game
    * @param data Elements of the scene
    * @param physics The physics engine
    * @return whether initialisation was successful
    */
    bool Initialise(SceneData& data, PhysicsEngine& physics);

    /**
    * Resets the game
    * @param data Elements of the scene
    * @param physics The physics engine
    * @return whether reset was successful
    */
    bool Reset(SceneData& data, PhysicsEngine& physics);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    * @param reset Callback to reset the tweak bar
    */
    void AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset);

    /**
    * Sends a request to fire the player gun
    */
    void FirePlayer();

    /**
    * Sends a request to flip the player over
    */
    void FlipPlayer();

    /**
    * Sends a request to rotate the player
    * @param left Whether to rotate left or right
    */
    void RotatePlayer(bool left);

    /**
    * Sends a request to rotate the player gun
    * @param left Whether to rotate left or right
    */
    void RotatePlayerGun(bool left);

    /**
    * Sends a request to move the player
    * @param forwards Whether to move forwards or backwards
    */
    void MovePlayer(bool forwards);

private:

    /**
    * Prevent copying
    */
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Camera& m_camera;                             ///< Main camera
    std::unique_ptr<GameBuilder> m_builder;       ///< Constructs the game
    std::unique_ptr<MovementUpdater> m_movement;  ///< Controls the movement of the tanks
    std::unique_ptr<BulletSpawner> m_spawner;     ///< Controls the spawning of bullets
    std::unique_ptr<GameData> m_data;             ///< Elements of the game
    int m_selectedEnemy = 0;                      ///< Currently selected enemy in the tweak bar
}; 
