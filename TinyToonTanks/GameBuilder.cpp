////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "GameBuilder.h"
#include "GameData.h"
#include "SceneData.h"
#include "BulletPhysics.h"
#include "Common.h"

GameBuilder::GameBuilder() = default;
GameBuilder::~GameBuilder() = default;

bool GameBuilder::Initialise(GameData& gamedata,
                             SceneData& scenedata, 
                             BulletPhysicsWorld& physics)
{
    return InitialiseTanks(gamedata, scenedata, physics) &&
        InitialiseBullets(gamedata, scenedata, physics);
}

bool GameBuilder::InitialiseTanks(GameData& gamedata,
                                  SceneData& scenedata, 
                                  BulletPhysicsWorld& physics)

{
    const int enemyHealth = 2;
    const int enemyDamage = 2;
    const int enemyAmount = 5;
    const int playerHealth = 6;
    const int playerDamage = 2;
    const glm::vec3 spawnPosition(0, 0, 0);
    const float spawnSize = 10.0f;
    const int spawnRows = 2;
    const int spawnColumns = 3;
	const float tankMass = 500.0f;
	const float gunMass = 20.0f;
	

    gamedata.player = std::make_unique<Player>();
    gamedata.enemies.push_back(std::make_unique<Enemy>());

    return true;
}

bool GameBuilder::InitialiseBullets(GameData& gamedata,
                                    SceneData& scenedata, 
                                    BulletPhysicsWorld& physics)

{
    const int bulletAmount = 25;
    const int bulletHealth = 2;
    const int bullletDamage = 2;
    const float bulletMass = 0.1f;

    return true;
}