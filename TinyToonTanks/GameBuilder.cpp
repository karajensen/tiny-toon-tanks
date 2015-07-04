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
    return InitialiseWorld(gamedata, scenedata, physics) &&
        InitialiseTanks(gamedata, scenedata, physics) &&
        InitialiseBullets(gamedata, scenedata, physics);
}

bool GameBuilder::InitialiseWorld(GameData& gamedata,
                                  SceneData& scenedata, 
                                  BulletPhysicsWorld& physics)
{
	const float groundHeight = -60.0f;
	const float floorHeight = groundHeight + 2.282f;
	const float wallDistance = 44.147f;
	const glm::vec3 wallBox(44.993f, groundHeight + 2.712f, 44.371f);

    scenedata.meshes[MeshID::GROUND]->Position(0.0f, groundHeight, 0.0f);
    scenedata.meshes[MeshID::WALL]->Position(0.0f, floorHeight, wallDistance, 0);
    scenedata.meshes[MeshID::WALL]->Position(0.0f, floorHeight, -wallDistance, 1);
    scenedata.meshes[MeshID::WALL]->Position(0.0f, -floorHeight, wallDistance, 2);
    scenedata.meshes[MeshID::WALL]->Position(0.0f, floorHeight, wallDistance, 3);
    scenedata.meshes[MeshID::WALL]->Rotation(0.0f, 90.0f, 0.0f, 2);
    scenedata.meshes[MeshID::WALL]->Rotation(0.0f, 90.0f, 0.0f, 3);
    scenedata.meshes[MeshID::WALLBOX]->Position(wallBox.x, wallBox.y, wallBox.z, 0);
    scenedata.meshes[MeshID::WALLBOX]->Position(-wallBox.x, wallBox.y, wallBox.z, 1);
    scenedata.meshes[MeshID::WALLBOX]->Position(wallBox.x, wallBox.y, -wallBox.z, 2);
    scenedata.meshes[MeshID::WALLBOX]->Position(-wallBox.x, wallBox.y, -wallBox.z, 3);

    return true;
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
	const float tankMass = 500.0f;
	const float gunMass = 20.0f;
	
    gamedata.player = std::make_unique<Player>();

    for (int i = 0; i < Instance::ENEMIES; ++i)
    {
        gamedata.enemies.push_back(std::make_unique<Enemy>());
    }

    int index = 0;
    const int spawnRows = 2;
    const int spawnColumns = 3;
    auto& tank = *scenedata.meshes[MeshID::TANK];
    auto& tankp1 = *scenedata.meshes[MeshID::TANKP1];
    auto& tankp2 = *scenedata.meshes[MeshID::TANKP2];
    auto& tankp3 = *scenedata.meshes[MeshID::TANKP3];
    auto& tankp4 = *scenedata.meshes[MeshID::TANKP4];
    auto& tankGun = *scenedata.meshes[MeshID::TANKGUN];

	for(int r = 0; r < spawnRows; r++)
	{
		for(int c = 0; c < spawnColumns; c++)
		{
            const float x = spawnPosition.x + (r * spawnSize);
            const float y = 0.0f;
            const float z = spawnPosition.z + (c * spawnSize);

            tank.Position(x, y, z, index);
            tankGun.Position(x, y, z, index);

            tankp1.Visible(false, index);
            tankp2.Visible(false, index);
            tankp3.Visible(false, index);
            tankp4.Visible(false, index);

            ++index;
		}
	}

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

    auto& bullet = *scenedata.meshes[MeshID::BULLET];
    for (int i = 0; i < bullet.Instances(); ++i)
    {
        bullet.Visible(false, i);
    }

    return true;
}