////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletSpawner.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "BulletSpawner.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "GlmHelper.h"

namespace
{
    const float HeightOffset = 2.11f;   ///< Height offset from the gun to fire at
    const float ForwardOffset = -5.0f;  ///< Forward offset from the gun to fire at
    const float RecoilImpulse = 45.0f;  ///< Amount of impulse the tank receives from firing
}

BulletSpawner::BulletSpawner(PhysicsEngine& physics, 
                             GameData& gameData,
                             SceneData& sceneData) :
    m_physics(physics),
    m_gameData(gameData),
    m_sceneData(sceneData)
{
}

BulletSpawner::~BulletSpawner() = default;

void BulletSpawner::Tick(float deltatime)
{
    FireBullet(*m_gameData.player);

    for (auto& enemy : m_gameData.enemies)
    {
        FireBullet(*enemy);
    }

    for (auto& bullet : m_gameData.bullets)
    {
        if (bullet->IsActive())
        {
            MoveBullet(*bullet);
        }
    }
}

void BulletSpawner::MoveBullet(Bullet& bullet)
{
    const int ID = bullet.GetPhysicsID();
	m_physics.SetInternalDamping(ID, 0, 0);
}

void BulletSpawner::FireBullet(const Tank& tank)
{
    if ((tank.GetMovementRequest() & Tank::FIRE) == Tank::FIRE)
    {
        for (auto& bullet : m_gameData.bullets)
        {
            if (!bullet->IsActive())
            {
                glm::mat4 world = tank.GetGunWorldMatrix();
                const glm::vec3 up = glm::matrix_get_up(world);
                const glm::vec3 forward = glm::matrix_get_forward(world);
                const glm::vec3 position = glm::matrix_get_position(world)
                    + (up * HeightOffset) + (forward * ForwardOffset);
                glm::matrix_set_position(world, position);

                bullet->SetActive(true);
                bullet->SetWorld(world);

                const int bulletID = bullet->GetPhysicsID();
                const int tankID = tank.GetPhysicsIDs().Body;
                m_physics.SetFriction(bulletID, 0);
	            m_physics.SetGravity(bulletID, 0.01f);
                m_physics.SetMotionState(bulletID, world);
                m_physics.AddToWorld(bulletID, true);
                m_physics.SetGroup(bulletID, m_physics.GetGroup(tankID));
                m_physics.ResetVelocityAndForce(bulletID);
                m_physics.AddImpulse(forward * RecoilImpulse, glm::vec3(0, 0, 0), tankID);

                return;
            }
        }
    }
}