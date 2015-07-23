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

    auto& ground = *scenedata.meshes[MeshID::GROUND];
    auto& wall = *scenedata.meshes[MeshID::WALL];
    auto& wallbox = *scenedata.meshes[MeshID::WALLBOX];

    ground.Position(0.0f, groundHeight, 0.0f);
    wall.Position(0.0f, floorHeight, wallDistance, 0);
    wall.Position(0.0f, floorHeight, -wallDistance, 1);
    wall.Position(wallDistance, floorHeight, 0.0f, 2);
    wall.Position(-wallDistance, floorHeight, 0.0f, 3);
    wall.Rotation(0.0f, 90.0f, 0.0f, 2);
    wall.Rotation(0.0f, 90.0f, 0.0f, 3);
    wallbox.Position(wallBox.x, wallBox.y, wallBox.z, 0);
    wallbox.Position(-wallBox.x, wallBox.y, -wallBox.z, 1);
    wallbox.Position(wallBox.x, wallBox.y, -wallBox.z, 2);
    wallbox.Position(-wallBox.x, wallBox.y, wallBox.z, 3);

    ground.UpdateTransforms();
    wall.UpdateTransforms();

    const int groundShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::GROUND]->VertexPositions());

    physics.LoadRigidBody(ground.GetWorld(), groundShape, 
        0.0f, m_collisionGroupIndex, MeshID::GROUND, 0, true);

    const int wallShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::WALL]->VertexPositions());

    for (int i = 0; i < 4; ++i)
    {
        physics.LoadRigidBody(wall.GetWorld(i), wallShape, 
            0.0f, m_collisionGroupIndex, MeshID::WALL, i, true);
    }

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
    const float tankPartMass = tankMass / 3.0f;
	const float gunMass = 20.0f;

    auto& tankBody = *scenedata.meshes[MeshID::TANK];
    auto& tankp1 = *scenedata.meshes[MeshID::TANKP1];
    auto& tankp2 = *scenedata.meshes[MeshID::TANKP2];
    auto& tankp3 = *scenedata.meshes[MeshID::TANKP3];
    auto& tankp4 = *scenedata.meshes[MeshID::TANKP4];
    auto& tankGun = *scenedata.meshes[MeshID::TANKGUN];

    gamedata.tankMesh = std::make_unique<TankMesh>(
        tankBody, tankGun, tankp1, tankp2, tankp3, tankp4);

    gamedata.player = std::make_unique<Player>(*gamedata.tankMesh, 0);
    for (int i = 1; i <= Instance::ENEMIES; ++i)
    {
        gamedata.enemies.push_back(
            std::make_unique<Enemy>(*gamedata.tankMesh, i));
    }

    int index = 0;
    const int spawnRows = 2;
    const int spawnColumns = 3;
	for(int r = 0; r < spawnRows; r++)
	{
		for(int c = 0; c < spawnColumns; c++)
		{
            const float x = spawnPosition.x + (r * spawnSize);
            const float y = 0.0f;
            const float z = spawnPosition.z + (c * spawnSize);

            tankBody.Position(x, y, z, index);
            tankGun.Position(x, y, z, index);

            tankp1.Visible(false, index);
            tankp2.Visible(false, index);
            tankp3.Visible(false, index);
            tankp4.Visible(false, index);

            ++index;
		}
	}

    tankBody.UpdateTransforms();
    tankGun.UpdateTransforms();
    tankp1.UpdateTransforms();
    tankp2.UpdateTransforms();
    tankp3.UpdateTransforms();
    tankp4.UpdateTransforms();

    // Initialise tank physics
    const int tankBodyShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANK]->VertexPositions());

    const int tankGunShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::GUN]->VertexPositions());

    const int tankP1Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP1]->VertexPositions());

    const int tankP2Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP2]->VertexPositions());

    const int tankP3Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP3]->VertexPositions());

    const int tankP4Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP4]->VertexPositions());

    auto CreateTankPhysics = [&](int instance) -> TankPhysicsIDs
    {
        TankPhysicsIDs IDs;

        ++m_collisionGroupIndex;

        IDs.Body = physics.LoadRigidBody(tankBody.GetWorld(instance), tankBodyShape, 
            tankMass, m_collisionGroupIndex, MeshID::TANK, instance, true);

        IDs.Gun = physics.LoadRigidBody(tankGun.GetWorld(instance), tankGunShape, 
            gunMass, m_collisionGroupIndex, MeshID::TANKGUN, instance, true);

        ++m_collisionGroupIndex;

        IDs.P1 = physics.LoadRigidBody(tankp1.GetWorld(instance), tankP1Shape, 
            tankPartMass, m_collisionGroupIndex, MeshID::TANKP1, instance, false,
            glm::vec3(-0.158f, 0.414f, 2.21f));

        ++m_collisionGroupIndex;

        IDs.P2 = physics.LoadRigidBody(tankp2.GetWorld(instance), tankP2Shape, 
            tankPartMass, m_collisionGroupIndex, MeshID::TANKP2, instance, false,
            glm::vec3(-0.158f, 0.345f, 0.191f));

        ++m_collisionGroupIndex;

        IDs.P3 = physics.LoadRigidBody(tankp3.GetWorld(instance), tankP3Shape, 
            tankPartMass, m_collisionGroupIndex, MeshID::TANKP3, instance, false,
            glm::vec3(-0.158f, 0.0f, -1.916f));

        ++m_collisionGroupIndex;

        IDs.P4 = physics.LoadRigidBody(tankp4.GetWorld(instance), tankP4Shape, 
            gunMass, m_collisionGroupIndex, MeshID::TANKP4, instance, false,
            glm::vec3(0.0f, 2.096f, 0.0f));

        physics.AddToWorld(IDs.P1, false);
        physics.AddToWorld(IDs.P2, false);
        physics.AddToWorld(IDs.P3, false);
        physics.AddToWorld(IDs.P4, false);
       
        IDs.Hinge = physics.CreateHinge(IDs.Body, IDs.Gun, 
            glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 
            glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

        return IDs;
    };

    gamedata.player->SetPhysicsIDs(CreateTankPhysics(0));
    for (int i = 0; i < Instance::ENEMIES; ++i)
    {
        gamedata.enemies[i]->SetPhysicsIDs(CreateTankPhysics(i+1));
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