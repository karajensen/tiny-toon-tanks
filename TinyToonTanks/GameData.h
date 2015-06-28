////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

#include "Player.h"
#include "Enemy.h"

/**
* Internal data for the game
*/
struct GameData
{
    GameData()
    {
    }

    ~GameData()
    {
    }

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
};