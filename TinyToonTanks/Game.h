////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Game.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Postprocessing.h"

class BulletPhysicsWorld;
class Tweaker;
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
    */
    Game();

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
    bool Initialise(SceneData& data, BulletPhysicsWorld& physics);

    /**
    * Reloads the scene
    */
    void Reload();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    * @param reset Callback to reset the tweak bar
    */
    void AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset);

private:

    /**
    * Prevent copying
    */
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    std::unique_ptr<GameData> m_data;  ///< Elements of the game
    int m_selectedEnemy = 0;           ///< Currently selected enemy in the tweak bar
}; 
