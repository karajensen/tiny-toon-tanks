////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ToonText.h"

#include <vector>
#include <memory>

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
    ToonText* toonText = nullptr;
};