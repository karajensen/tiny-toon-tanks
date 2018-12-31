////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Bullet.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"
#include "Mesh.h"
#include "GlmHelper.h"

#include <algorithm>

namespace
{
    const int InitialBulletHealth = 2; ///< Amount of initial health
    const int ImpulseCounterMax = 2;   ///< Maximum queries before impulse is needed
    const int DrawCounterMax = 3;      ///< Maximum queries before bullet can be drawn
}

Bullet::Bullet(Mesh& mesh, int instance) :
    m_mesh(mesh),
    m_instance(instance)
{  
    Reset();
}

void Bullet::SetPhysicsID(int ID)
{
    m_physicsID = ID;
}

int Bullet::GetPhysicsID() const
{
    return m_physicsID;
}

void Bullet::SetPhysicsGroupID(int ID)
{
    m_physicsGroupID = ID;
}

int Bullet::GetPhysicsGroupID() const
{
    return m_physicsGroupID;
}

void Bullet::Reset()
{
    SetIsAlive(false);
    m_health = InitialBulletHealth;
    m_friendlyFire = false;
    m_generateImpuse = false;
}

void Bullet::SetGenerateImpulse(bool generate, const glm::vec3& direction)
{
    m_generateImpuse = generate;
    m_generateImpulseDirection = direction;
}

bool Bullet::ShouldGenerateImpulse() const
{
    return m_generateImpuse;
}

const glm::vec3& Bullet::GetGeneratedImpulseDirection() const
{
    return m_generateImpulseDirection;
}

void Bullet::SetIsAlive(bool alive)
{
    m_alive = alive;
    m_mesh.Visible(m_alive, m_instance);
}

bool Bullet::IsAlive() const
{
    return m_alive;
}

void Bullet::SetWorld(const glm::mat4& world)
{
    m_mesh.SetWorld(world, m_instance);
}

void Bullet::SetAllowFriendlyFire(bool allow)
{
    m_friendlyFire = allow;
}

bool Bullet::AllowFriendlyFire() const
{
    return m_friendlyFire;
}

const glm::vec3& Bullet::GetPosition() const
{
    return m_mesh.Position(m_instance);
}

void Bullet::TakeDamage(int amount)
{
    m_health = std::max(0, m_health - amount);
}

int Bullet::Health() const
{
    return m_health;
}

const glm::vec3& Bullet::GetFiredPosition() const
{
    return m_firePosition;
}

void Bullet::SetFiredPosition(const glm::vec3& position)
{
    m_firePosition = position;
}