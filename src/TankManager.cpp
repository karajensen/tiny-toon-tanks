////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TankManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TankManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"
#include "GlmHelper.h"
#include "Utils.h"

namespace
{
    const float TANK_GUN_DAMPING = 0.001f;
    const float TANK_GUN_ROT_AMOUNT = 800.0f;
    const float TANK_FORCE_DAMPING = 0.001f;
    const float TANK_ROT_DAMPING = 0.000015f;
    const float TANK_FORCE_AMOUNT = 20000.0f;
    const float TANK_AI_MAX_COUNTER = 3000.0f;
    const float TANK_TANK_AI_THRESHOLD = 50.0f;

    /**
    * Offsets from the tank center to apply movement forces to
    */
    const glm::vec3 BackTopLeft(-1.709f, 1.926f, 3.233f);
    const glm::vec3 BackTopRight(1.709f, 1.926f, 3.233f);
    const glm::vec3 BackBotLeft(-1.709f, -1.076f, 3.233f);
    const glm::vec3 BackBotRight(1.709f, -1.076f, 3.233f);
    const glm::vec3 FrontTopLeft(-1.709f, 1.926f, -3.238f);
    const glm::vec3 FrontTopRight(1.709f, 1.926f, -3.238f);
    const glm::vec3 FrontBotLeft(-1.709f, -1.076f, -3.238f);
    const glm::vec3 FrontBotRight(1.709f, -1.076f, -3.238f);
}

TankManager::TankManager(PhysicsEngine& physics,
                                         GameData& gameData,
                                         SceneData& sceneData)
    : m_physics(physics)
    , m_gameData(gameData)
    , m_sceneData(sceneData)
{
}

TankManager::~TankManager() = default;

void TankManager::PrePhysicsTick(float physicsDeltaTime)
{
    UpdateTankMovement(physicsDeltaTime, *m_gameData.player);
    UpdateGunMovement(physicsDeltaTime, *m_gameData.player);

    const auto& playerWorld = m_gameData.player->GetWorldMatrix();
    for (auto& enemy : m_gameData.enemies)
    {
        UpdateTankMovement(physicsDeltaTime, *enemy);
        UpdateGunMovement(physicsDeltaTime, *enemy);
    } 
}

void TankManager::PostPhysicsTick()
{
    UpdateTankPositions(*m_gameData.player);

    for (auto& enemy : m_gameData.enemies)
    {
        UpdateTankPositions(*enemy);
    }
}

void TankManager::UpdateTankPositions(const Tank& tank)
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

void TankManager::UpdateTankMovement(float physicsDeltaTime, Tank& tank)
{
    const auto ID = tank.GetPhysicsIDs().Body;
    const unsigned int request = tank.GetMovementRequest();
    const bool moveBack = (request & Tank::BACKWARDS) == Tank::BACKWARDS;
    const bool moveForward = (request & Tank::FORWARDS) == Tank::FORWARDS;
    const bool moveRight = (request & Tank::ROTATE_RIGHT) == Tank::ROTATE_RIGHT;
    const bool moveLeft = (request & Tank::ROTATE_LEFT) == Tank::ROTATE_LEFT;

    const auto& world = tank.GetWorldMatrix();
    const auto basis = glm::mat3(world);
    auto forward = glm::matrix_get_forward(world);
    auto right = glm::matrix_get_right(world);
    forward *= TANK_FORCE_AMOUNT * physicsDeltaTime;

    const float turnAmountLarge = (TANK_FORCE_AMOUNT * physicsDeltaTime) / 4.0f;
    const float turnAmountSmall = (TANK_FORCE_AMOUNT * physicsDeltaTime) / 8.0f;
    auto rightLarge = right * turnAmountLarge;
    auto rightSmall = right * turnAmountSmall;

    bool addedlinearforce = false;
    bool addedrotforce = false;

    if (moveBack)
    {
        tank.SetLinearDamping(1.0f);
        addedlinearforce = true;
        m_physics.AddForce(forward, basis * BackBotLeft, ID);
        m_physics.AddForce(forward, basis * BackBotRight, ID);
        m_physics.AddForce(forward, basis * BackTopLeft, ID);
        m_physics.AddForce(forward, basis * BackTopRight, ID);
    }
    if (moveForward)
    {
        tank.SetLinearDamping(1.0f);
        addedlinearforce = true;
        m_physics.AddForce(-forward, basis * FrontBotLeft, ID);
        m_physics.AddForce(-forward, basis * FrontBotRight, ID);
        m_physics.AddForce(-forward, basis * FrontTopLeft, ID);
        m_physics.AddForce(-forward, basis * FrontTopRight, ID);
    }
    if (moveRight)
    {
        tank.SetRotationalDamping(1.0f);
        addedrotforce = true;
        m_physics.AddForce(rightLarge, basis * FrontBotLeft, ID);
        m_physics.AddForce(rightSmall, basis * FrontBotRight, ID);
        m_physics.AddForce(rightLarge, basis * FrontTopLeft, ID);
        m_physics.AddForce(rightSmall, basis * FrontTopRight, ID);
    }
    if (moveLeft)
    {
        tank.SetRotationalDamping(1.0f);
        addedrotforce = true;
        m_physics.AddForce(-rightSmall, basis * FrontBotLeft, ID);
        m_physics.AddForce(-rightLarge, basis * FrontBotRight, ID);
        m_physics.AddForce(-rightSmall, basis * FrontTopLeft, ID);
        m_physics.AddForce(-rightLarge, basis * FrontTopRight, ID);
    }

    // If no movement, dampen current movement
    if (!addedrotforce)
    {
        float damping = tank.GetRotationalDamping() - (TANK_ROT_DAMPING * physicsDeltaTime);
        if (damping <= 0.0f)
        {
            damping = 0.0f;
        }
        tank.SetRotationalDamping(damping);
        m_physics.AddRotationalDamping(ID, damping);
    }

    if (!addedlinearforce && !tank.IsDropping())
    {
        float damping = tank.GetLinearDamping() - (TANK_FORCE_DAMPING * physicsDeltaTime);
        if (damping <= 0.0f)
        {
            damping = 0.0f;
        }
        tank.SetLinearDamping(damping);
        m_physics.AddLinearDamping(ID, damping);
    }
}

void TankManager::UpdateGunMovement(float physicsDeltaTime, Tank& tank)
{
    const unsigned int request = tank.GetMovementRequest();
    const bool rotateLeft = (request & Tank::GUN_LEFT) == Tank::GUN_LEFT;
    const bool rotateRight = (request & Tank::GUN_RIGHT) == Tank::GUN_RIGHT;

    const int hinge = tank.GetPhysicsIDs().Hinge;
    float lastTarget = RadToDeg(m_physics.GetHingeRotation(hinge));

    if (rotateRight)
    {
        tank.SetGunRotationalDamping(1.0f);
        lastTarget += TANK_GUN_ROT_AMOUNT * physicsDeltaTime;
        m_physics.RotateHinge(hinge, DegToRad(lastTarget), 1.0f);
    }
    else if (rotateLeft)
    {
        tank.SetGunRotationalDamping(1.0f);
        lastTarget -= TANK_GUN_ROT_AMOUNT * physicsDeltaTime;
        m_physics.RotateHinge(hinge, DegToRad(lastTarget), 1.0f);
    }
    else
    {
        float damping = tank.GetGunRotationalDamping() - (TANK_GUN_DAMPING * physicsDeltaTime);
        if (damping <= 0.0f)
        {
            damping = 0.0f;
        }
        tank.SetGunRotationalDamping(damping);
        m_physics.StopHinge(hinge, 1.0f, damping);
    }
}