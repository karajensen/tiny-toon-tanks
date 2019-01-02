////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GameBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "GameBuilder.h"
#include "GameData.h"
#include "SceneData.h"
#include "PhysicsEngine.h"
#include "CollisionManager.h"
#include "Common.h"

GameBuilder::GameBuilder() = default;
GameBuilder::~GameBuilder() = default;

bool GameBuilder::Initialise(GameData& gamedata,
                             SceneData& scenedata, 
                             PhysicsEngine& physics,
                             CollisionManager& collisionManager)
{
    physics.ResetSimulation();

    return InitialiseWorld(gamedata, scenedata, physics, collisionManager) &&
        InitialiseTanks(gamedata, scenedata, physics, collisionManager) &&
        InitialiseBullets(gamedata, scenedata, physics, collisionManager);
}

bool GameBuilder::InitialiseWorld(GameData& gamedata,
                                  SceneData& scenedata, 
                                  PhysicsEngine& physics,
                                  CollisionManager& collisionManager)
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

    gamedata.wallNormals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    gamedata.wallNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    gamedata.wallNormals.push_back(glm::vec3(-1.0f, 0.0, 0.0f));
    gamedata.wallNormals.push_back(glm::vec3(1.0f, 0.0, 0.0f));

    ground.UpdateTransforms();
    wall.UpdateTransforms();

    const int environmentGroup = collisionManager.GetCollisionGroupIndex();

    const int groundShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::GROUND]->VertexPositions());
    assert(groundShape == HullID::GROUND);

    physics.LoadRigidBody(ground.GetWorld(), groundShape, 
        0.0f, environmentGroup, MeshID::GROUND, 0, true);

    const int wallShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::WALL]->VertexPositions());
    assert(wallShape == HullID::WALL);

    for (int i = 0; i < Instance::TANK_PIECES; ++i)
    {
        physics.LoadRigidBody(wall.GetWorld(i), wallShape, 
            0.0f, environmentGroup, MeshID::WALL, i, true);
    }

    gamedata.toonText = dynamic_cast<ToonText*>(scenedata.quads[QuadID::TOONTEXT].get());
    return gamedata.toonText != nullptr;
}

bool GameBuilder::InitialiseTanks(GameData& gamedata,
                                  SceneData& scenedata, 
                                  PhysicsEngine& physics,
                                  CollisionManager& collisionManager)

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

    if (!gamedata.tankMesh)
    {
        gamedata.tankMesh = std::make_unique<Tank::MeshGroup>(
            tankBody, tankGun, tankp1, tankp2, tankp3, tankp4);

        for (int i = 0; i < Instance::ENEMIES; ++i)
        {
            gamedata.enemies.push_back(
                std::make_unique<Enemy>(*gamedata.tankMesh, i));
        }
        gamedata.player = std::make_unique<Player>(*gamedata.tankMesh, Instance::PLAYER);
    }
    else
    {
        gamedata.player->Reset();
        for (auto& enemy : gamedata.enemies)
        {
            enemy->Reset();
        }
    }

    int index = Instance::PLAYER;
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

            --index;
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
    assert(tankBodyShape == HullID::TANK);

    const int tankGunShape = physics.LoadConvexShape(
        scenedata.hulls[HullID::GUN]->VertexPositions());
    assert(tankGunShape == HullID::GUN);

    const int tankP1Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP1]->VertexPositions());
    assert(tankP1Shape == HullID::TANKP1);

    const int tankP2Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP2]->VertexPositions());
    assert(tankP2Shape == HullID::TANKP2);

    const int tankP3Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP3]->VertexPositions());
    assert(tankP3Shape == HullID::TANKP3);

    const int tankP4Shape = physics.LoadConvexShape(
        scenedata.hulls[HullID::TANKP4]->VertexPositions());
    assert(tankP4Shape == HullID::TANKP4);

    auto CreateTankPhysics = [&](int instance) -> Tank::PhysicsIDs
    {
        Tank::PhysicsIDs IDs;

        collisionManager.IncrementCollisionGroupIndex();

        IDs.Body = physics.LoadRigidBody(tankBody.GetWorld(instance), tankBodyShape, 
            tankMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANK, instance, true);

        IDs.Gun = physics.LoadRigidBody(tankGun.GetWorld(instance), tankGunShape, 
            gunMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANKGUN, instance, true);

        collisionManager.IncrementCollisionGroupIndex();

        IDs.P1 = physics.LoadRigidBody(tankp1.GetWorld(instance), tankP1Shape, 
            tankPartMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANKP1, instance, false,
            glm::vec3(-0.158f, 0.414f, 2.21f));

        collisionManager.IncrementCollisionGroupIndex();

        IDs.P2 = physics.LoadRigidBody(tankp2.GetWorld(instance), tankP2Shape, 
            tankPartMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANKP2, instance, false,
            glm::vec3(-0.158f, 0.345f, 0.191f));

        collisionManager.IncrementCollisionGroupIndex();

        IDs.P3 = physics.LoadRigidBody(tankp3.GetWorld(instance), tankP3Shape, 
            tankPartMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANKP3, instance, false,
            glm::vec3(-0.158f, 0.0f, -1.916f));

        collisionManager.IncrementCollisionGroupIndex();

        IDs.P4 = physics.LoadRigidBody(tankp4.GetWorld(instance), tankP4Shape, 
            gunMass, collisionManager.GetCollisionGroupIndex(), MeshID::TANKP4, instance, false,
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

    for (int i = 0; i < Instance::ENEMIES; ++i)
    {
        gamedata.enemies[i]->SetPhysicsIDs(CreateTankPhysics(i));
    }
    gamedata.player->SetPhysicsIDs(CreateTankPhysics(Instance::PLAYER));

    return true;
}

bool GameBuilder::InitialiseBullets(GameData& gamedata,
                                    SceneData& scenedata, 
                                    PhysicsEngine& physics,
                                    CollisionManager& collisionManager)

{
    const int bulletHealth = 2;
    const int bullletDamage = 2;
    const float bulletMass = 0.1f;
    const bool createBullets = gamedata.bullets.empty();

    auto& bullet = *scenedata.meshes[MeshID::BULLET];
    const int shape = physics.LoadConvexShape(bullet.VertexPositions());
    assert(shape == HullID::BULLET);

    for (int i = 0; i < bullet.Instances(); ++i)
    {
        if (createBullets)
        {
            gamedata.bullets.push_back(
                std::make_unique<Bullet>(bullet, i));
        }

        collisionManager.IncrementCollisionGroupIndex();
        const auto groupID = collisionManager.GetCollisionGroupIndex();

        const int ID = physics.LoadRigidBody(glm::mat4(), shape, 
            bulletMass, groupID, MeshID::BULLET, i, true);
        
        gamedata.bullets[i]->SetIsAlive(false);
        gamedata.bullets[i]->SetPhysicsID(ID);
        physics.AddToWorld(ID, false);
    }

    return true;
}