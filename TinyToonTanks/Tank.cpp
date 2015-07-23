////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Tank.h"
#include "Mesh.h"
#include "Tweaker.h"
#include "TankData.h"

Tank::Tank(TankMesh& tankmesh, int instance) :
    m_instance(instance),
    m_tankmesh(tankmesh)
{
}

Tank::~Tank()
{
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

void Tank::SetPhysicsIDs(TankPhysicsIDs IDs)
{
    m_physicsIDs = IDs;
}

const TankPhysicsIDs& Tank::GetPhysicsIDs() const
{
    return m_physicsIDs;
}

bool Tank::IsAlive() const
{
    return m_alive; 
}