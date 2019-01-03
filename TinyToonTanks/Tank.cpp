////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Tank.h"
#include "Mesh.h"
#include "Tweaker.h"
#include <algorithm>

namespace
{
    const float FIRE_GUN_DELAY = 500.0f;  ///< Ms to delay before allowing firing again
    const int INITIAL_TANK_HEALTH = 2;     ///< Amount of initial tank health
}

Tank::Tank(MeshGroup& tankmesh, int instance) :
    m_instance(instance),
    m_tankmesh(tankmesh)
{
}

Tank::~Tank()
{
}

void Tank::Reset()
{
    SetIsAlive(true);
    m_movement = NO_MOVEMENT;      
    m_previousMovement = NO_MOVEMENT;
    m_linearDamping = 1.0f; 
    m_rotationalDamping = 1.0f; 
    m_gunDamping = 2.0f;
    m_fireGunTime = 0.0f;
    m_isDropping = true;
    m_health = INITIAL_TANK_HEALTH;
}

void Tank::SetDropping(bool dropping)
{
    m_isDropping = dropping;
}

bool Tank::IsDropping() const
{
    return m_isDropping;
}

void Tank::Update(float deltatime)
{
    if (m_fireGunTime > 0.0f)
    {
        m_fireGunTime -= deltatime;
    }
}

int Tank::GetInstance() const
{
    return m_instance;
}

void Tank::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddButton("Kill Tank", [this]()
    {
        SetIsAlive(false);
    });
}

void Tank::SetIsAlive(bool alive)
{
    m_alive = alive;
    m_tankmesh.Body.SetVisible(m_alive, m_instance);
    m_tankmesh.Gun.SetVisible(m_alive, m_instance);
    m_tankmesh.P1.SetVisible(!m_alive, m_instance);
    m_tankmesh.P2.SetVisible(!m_alive, m_instance);
    m_tankmesh.P3.SetVisible(!m_alive, m_instance);
    m_tankmesh.P4.SetVisible(!m_alive, m_instance);
}

void Tank::SetPieceWorldMatrix(MeshID::ID id, const glm::mat4& matrix)
{
    switch (id)
    {
    case MeshID::TANKP1:
        m_tankmesh.P1.SetWorld(matrix, m_instance);
        break;
    case MeshID::TANKP2:
        m_tankmesh.P2.SetWorld(matrix, m_instance);
        break;
    case MeshID::TANKP3:
        m_tankmesh.P3.SetWorld(matrix, m_instance);
        break;
    case MeshID::TANKP4:
        m_tankmesh.P4.SetWorld(matrix, m_instance);
        break;
    }
}

const glm::vec3& Tank::GetPosition() const
{
    return m_tankmesh.Body.Position(m_instance);
}

void Tank::SetPhysicsIDs(PhysicsIDs IDs)
{
    m_physicsIDs = IDs;
}

const Tank::PhysicsIDs& Tank::GetPhysicsIDs() const
{
    return m_physicsIDs;
}

bool Tank::IsAlive() const
{
    return m_alive; 
}

void Tank::Fire()
{
    if (m_alive && m_fireGunTime <= 0.0f)
    {
        m_movement |= FIRE;
        m_fireGunTime = FIRE_GUN_DELAY;
    }
}

void Tank::Rotate(bool left)
{
    if (m_alive)
    {
        m_movement |= left ? ROTATE_LEFT : ROTATE_RIGHT;
    }
}

void Tank::RotateGun(bool left)
{
    if (m_alive)
    {
        m_movement |= left ? GUN_LEFT : GUN_RIGHT;
    }
}

void Tank::Move(bool forwards)
{
    if (m_alive)
    {
        m_movement |= forwards ? FORWARDS : BACKWARDS;
    }
}

unsigned int Tank::GetMovementRequest() const
{
    return m_movement;
}

unsigned int Tank::GetPreviousMovementRequest() const
{
    return m_previousMovement;
}

void Tank::ResetMovementRequest()
{
    m_previousMovement = m_movement;
    m_movement = NO_MOVEMENT;
}

void Tank::SetLinearDamping(float value)
{
    m_linearDamping = std::max(0.0f, value);
}

void Tank::AddLinearDamping(float value)
{
    m_linearDamping = std::max(0.0f, m_linearDamping + value);
}

float Tank::GetLinearDamping() const
{
    return m_linearDamping;
}

void Tank::SetRotationalDamping(float value)
{
    m_rotationalDamping = std::max(0.0f, value);
}

void Tank::AddRotationalDamping(float value)
{
    m_rotationalDamping = std::max(0.0f, m_rotationalDamping + value);
}

float Tank::GetRotationalDamping() const
{
    return m_rotationalDamping;
}

void Tank::SetGunRotationalDamping(float value)
{
    m_gunDamping = std::max(0.0f, value);
}

void Tank::AddGunRotationalDamping(float value)
{
    m_gunDamping = std::max(0.0f, m_gunDamping + value);
}

float Tank::GetGunRotationalDamping() const
{
    return m_gunDamping;
}

const glm::mat4& Tank::GetWorldMatrix() const
{
    return m_tankmesh.Body.GetWorld(m_instance);
}

const glm::mat4& Tank::GetGunWorldMatrix() const
{
    return m_tankmesh.Gun.GetWorld(m_instance);
}

void Tank::TakeDamage(int amount)
{
    m_health = std::max(0, m_health - amount);
}

int Tank::Health() const
{
    return m_health;
}