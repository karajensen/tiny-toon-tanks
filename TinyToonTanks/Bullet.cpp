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
    const int BulletDamageDealt = 2;   ///< Amount of damage a bullet deals
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

void Bullet::Reset()
{
    SetIsAlive(false);
    m_justShot = false;
    m_needsImpulse = false;
    m_health = InitialBulletHealth;
    m_allowScore = false;
    m_impulseCounter = 0;
}

void Bullet::SetNeedsImpulse(bool needsImpulse)
{
    m_needsImpulse = needsImpulse;
    m_impulseCounter = 0;
}

bool Bullet::NeedsImpulse()
{
    if (m_needsImpulse)
    {
        ++m_impulseCounter;
        if (m_impulseCounter >= ImpulseCounterMax)
        {
            SetNeedsImpulse(false);
            return true;
        }
    }
    return false;
}

void Bullet::SetIsAlive(bool alive)
{
    m_alive = alive;

    if (alive)
    {
        m_usingDrawCounter = alive;
        m_drawCounter = 0;
    }
    else
    {
        m_mesh.Visible(false, m_instance);
    }
}

bool Bullet::IsAlive() const
{
    return m_alive;
}

void Bullet::SetWorld(const glm::mat4& world)
{
    m_mesh.SetWorld(world, m_instance);
}

void Bullet::SetJustShot(bool justShot)
{
    m_justShot = justShot;
}

bool Bullet::JustShot() const
{
    return m_justShot;
}

void Bullet::SetAllowScore(bool allowScore)
{
    m_allowScore = allowScore;
}

bool Bullet::AllowScore() const
{
    return m_allowScore;
}

const glm::vec3& Bullet::Position() const
{
    return m_mesh.Position(m_instance);
}

void Bullet::TakeDamage(int amount)
{
    m_health = std::max(0, m_health - amount);
}

int Bullet::DamageDealt() const
{
    return BulletDamageDealt;
}

int Bullet::Health() const
{
    return m_health;
}

glm::vec3 Bullet::Forward() const
{
    return glm::matrix_get_forward(m_mesh.GetWorld(m_instance));
}

bool Bullet::UsingDrawCounter() const
{
    return m_usingDrawCounter;
}

void Bullet::UpdateDrawCounter()
{
    if (m_alive && m_usingDrawCounter)
    {
        if (m_drawCounter < DrawCounterMax)
        {
            ++m_drawCounter;
        }
        else
        {
            m_usingDrawCounter = false;
            m_drawCounter = 0;
            m_mesh.Visible(true, m_instance);
        }
    }
}