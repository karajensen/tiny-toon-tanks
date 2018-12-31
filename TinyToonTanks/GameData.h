////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

/**
* Internal data for the game
*/
struct GameData
{
    std::unique_ptr<Tank::MeshGroup> tankMesh;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<glm::vec3> wallNormals;
};