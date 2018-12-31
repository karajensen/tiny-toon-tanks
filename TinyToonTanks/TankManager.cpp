////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TankManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "TankManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"
#include "GlmHelper.h"
#include "Common.h"

namespace
{
    const float ForwardForce = 3100.0f;        ///< Forward force to add for movement
    const float RotationForceFar = 500.0f;     ///< Force to apply at the far corners
    const float RotationForceClose = 100.0f;   ///< Force to apply at the near corners
    const float GunRotation = 1500.0f;         ///< Amount to apply for rotating the gun
    const float LinearDamping = 0.0005f;       ///< Linear damping amount for movement
    const float RotationalDamping = 0.00002f;  ///< Rotational damping amount for rotating
    const float GunRotationDamping = 0.4f;     ///< Rotation damping for the gun hinge

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
                                         SceneData& sceneData) :
    m_physics(physics),
    m_gameData(gameData),
    m_sceneData(sceneData)
{
}

TankManager::~TankManager() = default;

void TankManager::PrePhysicsTick(float deltatime)
{
    UpdateTankMovement(deltatime, *m_gameData.player);
    UpdateGunMovement(deltatime, *m_gameData.player);

    for (auto& enemy : m_gameData.enemies)
    {
        UpdateTankMovement(deltatime, *enemy);
        UpdateGunMovement(deltatime, *enemy);
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

void TankManager::UpdateTankMovement(float deltatime, Tank& tank)
{
    const int rb = tank.GetPhysicsIDs().Body;
    const unsigned int request = tank.GetMovementRequest();
    const bool forwards = (request & Tank::FORWARDS) == Tank::FORWARDS;
    const bool backwards = (request & Tank::BACKWARDS) == Tank::BACKWARDS;
    const bool rotateLeft = (request & Tank::ROTATE_LEFT) == Tank::ROTATE_LEFT;
    const bool rotateRight = (request & Tank::ROTATE_RIGHT) == Tank::ROTATE_RIGHT;

    if (forwards || backwards)
    {
        const glm::mat4& world = tank.GetWorldMatrix();
        const glm::mat3 basis(world);
        const glm::vec3 forward(glm::matrix_get_forward(world));
        const float direction = forwards ? -1.0f : 1.0f;
        const glm::vec3 force = forward * ForwardForce * direction;

        m_physics.AddForce(force, basis * (forwards ? FrontBotLeft : BackBotLeft), rb);
        m_physics.AddForce(force, basis * (forwards ? FrontBotRight : BackBotRight), rb);
        m_physics.AddForce(force, basis * (forwards ? FrontTopLeft : BackTopLeft), rb);
        m_physics.AddForce(force, basis * (forwards ? FrontTopRight : BackTopRight), rb);
        tank.SetLinearDamping(1.0f);
    }
    else
    {
        tank.AddLinearDamping(-deltatime * LinearDamping);
        m_physics.AddLinearDamping(rb, tank.GetLinearDamping());
    }

    if (rotateLeft || rotateRight)
    {
        const glm::mat4& world = tank.GetWorldMatrix();
        const glm::mat3 basis(world);
        const glm::vec3 right(glm::matrix_get_right(world));
        const float direction = rotateRight ? 1.0f : -1.0f;
        const glm::vec3 forceClose = right * RotationForceClose * direction;
        const glm::vec3 forceFar = right * RotationForceFar * direction;

        m_physics.AddForce(rotateRight ? forceFar : forceClose, basis * FrontBotLeft, rb);
        m_physics.AddForce(rotateRight ? forceClose : forceFar, basis * FrontBotRight, rb);
        m_physics.AddForce(rotateRight ? forceFar : forceClose, basis * FrontTopLeft, rb);
        m_physics.AddForce(rotateRight ? forceClose : forceFar, basis * FrontTopRight, rb);
        tank.SetRotationalDamping(1.0f);
    }
    else
    {
        tank.AddRotationalDamping(-deltatime * RotationalDamping);
        m_physics.AddRotationalDamping(rb, tank.GetRotationalDamping());
    }
}

void TankManager::UpdateGunMovement(float deltatime, Tank& tank)
{
    const unsigned int request = tank.GetMovementRequest();
    const int hinge = tank.GetPhysicsIDs().Hinge;
    const bool rotateLeft = (request & Tank::GUN_LEFT) == Tank::GUN_LEFT;
    const bool rotateRight = (request & Tank::GUN_RIGHT) == Tank::GUN_RIGHT;

    if(rotateLeft || rotateRight)
    {
        const float rotation = RadToDeg(m_physics.GetHingeRotation(hinge))
            + ((rotateLeft ? GunRotation : -GunRotation) * deltatime);
       
        m_physics.RotateHinge(hinge, DegToRad(rotation), 1.0f);
        tank.SetGunRotationalDamping(1.0f);
    }
    else
    {
        tank.AddGunRotationalDamping(-GunRotationDamping * deltatime);
        m_physics.StopHinge(hinge, 1.0f, tank.GetGunRotationalDamping());
    }
}