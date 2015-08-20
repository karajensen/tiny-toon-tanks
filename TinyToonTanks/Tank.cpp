////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Tank.h"
#include "Mesh.h"
#include "Tweaker.h"
#include <algorithm>

namespace
{
    float FIRE_GUN_DELAY = 0.5f; ///< Seconds to delay before allowing firing again
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
    m_alive = true;
    m_movement = NO_MOVEMENT;      
    m_linearDamping = 1.0f; 
    m_rotationalDamping = 1.0f; 
    m_gunDamping = 1.0f;
    m_fireGunTime = 0.0f;
}

void Tank::Update(float deltatime)
{
    m_fireGunTime = std::max(0.0f, m_fireGunTime - deltatime);
}

int Tank::GetInstance() const
{
    return m_instance;
}

void Tank::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddButton("Kill Tank", [this]()
    {
        m_alive = false;
    });
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
    if (m_fireGunTime == 0.0f)
    {
        m_movement |= FIRE;
        m_fireGunTime = FIRE_GUN_DELAY;
    }
}

void Tank::Flip()
{
    m_movement |= FLIP;
}

void Tank::Rotate(bool left)
{
    m_movement |= left ? ROTATE_LEFT : ROTATE_RIGHT;
}

void Tank::RotateGun(bool left)
{
    m_movement |= left ? GUN_LEFT : GUN_RIGHT;
}

void Tank::Move(bool forwards)
{
    m_movement |= forwards ? FORWARDS : BACKWARDS;
}

unsigned int Tank::GetMovementRequest() const
{
    return m_movement;
}

void Tank::ResetMovementRequest()
{
    m_movement = NO_MOVEMENT;;
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