////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PhysicsUpdater.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "PhysicsUpdater.h"
#include "BulletPhysics.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"

PhysicsUpdater::PhysicsUpdater(BulletPhysicsWorld& physics, 
                               GameData& gameData,
                               SceneData& sceneData) :
    m_physics(physics),
    m_gameData(gameData),
    m_sceneData(sceneData)
{
}

PhysicsUpdater::~PhysicsUpdater() = default;

void PhysicsUpdater::Tick()
{
    UpdateTankPositions(*m_gameData.player);
    for (auto& enemy : m_gameData.enemies)
    {
        UpdateTankPositions(*enemy);
    } 
}

void PhysicsUpdater::UpdateTankPositions(const Tank& tank)
{
    const int instance = tank.GetInstance();
    const auto& physicsIDs = tank.GetPhysicsIDs();

    if (tank.IsAlive())
    {
        m_sceneData.meshes[MeshID::TANK]->SetWorld(
            m_physics.GetTransform(physicsIDs.Body), instance);

        m_sceneData.meshes[MeshID::TANKGUN]->SetWorld(
            m_physics.GetTransform(physicsIDs.Gun), instance);
    }
    else
    {
        m_sceneData.meshes[MeshID::TANKP1]->SetWorld(
            m_physics.GetTransform(physicsIDs.P1), instance);

        m_sceneData.meshes[MeshID::TANKP2]->SetWorld(
            m_physics.GetTransform(physicsIDs.P2), instance);

        m_sceneData.meshes[MeshID::TANKP3]->SetWorld(
            m_physics.GetTransform(physicsIDs.P3), instance);

        m_sceneData.meshes[MeshID::TANKP4]->SetWorld(
            m_physics.GetTransform(physicsIDs.P4), instance);
    }
}