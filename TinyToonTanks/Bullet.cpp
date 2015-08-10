////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Bullet.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"
#include "Mesh.h"

Bullet::Bullet(Mesh& mesh, int instance) :
    m_mesh(mesh),
    m_instance(instance)
{  
}

void Bullet::SetPhysicsID(int ID)
{
    m_physicsID = ID;
}

int Bullet::GetPhysicsID() const
{
    return m_physicsID;
}

void Bullet::SetActive(bool active)
{
    m_mesh.Visible(active, m_instance);
}

bool Bullet::IsActive() const
{
    return m_mesh.IsVisible(m_instance);
}

void Bullet::SetWorld(const glm::mat4& world)
{
    m_mesh.SetWorld(world, m_instance);
}