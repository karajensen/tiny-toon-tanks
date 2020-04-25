////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "BulletManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "GlmHelper.h"
#include "SoundEngine.h"

namespace
{
    const float HEIGHT_OFFSET = 2.11f;                   ///< Height offset from the gun to fire at
    const float FORWARD_OFFSET = -5.0f;                  ///< Forward offset from the gun to fire at
    const float BULLET_FRIENDLY_FIRE_DISTANCE = 2.5f;    ///< Distance before allowing a bullet to self-score
    const int BULLET_WORLD_MAX_DISTANCE = 100;           ///< Maximum distance the bullet can travel before killed
    const float RECOIL_IMPULSE = 1000.0f;                ///< Amount of impulse the tank receives from firing
    const float BULLET_IMPULSE = 2.0f;                  ///< Amount of impulse the bullet recieves
}

BulletManager::BulletManager(PhysicsEngine& physics,
                             GameData& gameData,
                             SceneData& sceneData)
    : m_physics(physics)
    , m_gameData(gameData)
    , m_sceneData(sceneData)
{
}

BulletManager::~BulletManager() = default;

void BulletManager::PrePhysicsTick()
{
    FireBullet(*m_gameData.player);

    for (auto& enemy : m_gameData.enemies)
    {
        FireBullet(*enemy);
    }

    for (auto& bullet : m_gameData.bullets)
    {
        if (bullet->IsAlive())
        {
            MoveBullet(*bullet);
        }
    }
}

void BulletManager::PostPhysicsTick()
{
    for (auto& bullet : m_gameData.bullets)
    {
        if (bullet->IsAlive())
        {
            UpdateButtonPosition(*bullet);
        }
    }
}

void BulletManager::UpdateButtonPosition(Bullet& bullet)
{
    const int ID = bullet.GetPhysicsID();

    bullet.SetWorld(m_physics.GetTransform(ID));
}

void BulletManager::MoveBullet(Bullet& bullet)
{
    const int ID = bullet.GetPhysicsID();
    
    m_physics.SetInternalDamping(ID, 0.0f, 0.0f);
    m_physics.SetFriction(ID, 0);
    m_physics.SetGravity(ID, 0.0f);

    const auto distance = glm::length(bullet.GetPosition()); // Length from center to bullet
    glm::vec3 velocity = m_physics.GetVelocity(ID);

    if (distance >= BULLET_WORLD_MAX_DISTANCE || glm::vec3_is_zero(velocity))
    {
        bullet.SetIsAlive(false);
        m_physics.AddToWorld(ID, false);
        m_physics.ResetVelocityAndForce(ID);
        return;
    }

    if (bullet.ShouldGenerateImpulse())
    {
        const auto direction = bullet.GetGeneratedImpulseDirection();
        bullet.SetGenerateImpulse(false, glm::vec3());
        m_physics.ResetVelocityAndForce(ID);
        m_physics.SetVelocity(direction * glm::length(velocity), ID);
    }
    
    // Generate continous force
    m_physics.AddForce(glm::normalize(velocity) *
        BULLET_IMPULSE, glm::vec3(0, 0, 0), ID);
}

void BulletManager::FireBullet(const Tank& tank)
{
    if ((tank.GetMovementRequest() & Tank::FIRE) == Tank::FIRE)
    {
        for (auto& bullet : m_gameData.bullets)
        {
            if (!bullet->IsAlive())
            {
                bullet->Reset();
                bullet->SetIsAlive(true);

                SoundEngine::PlaySoundEffect(SoundEngine::SHOOT);

                glm::mat4 world = tank.GetGunWorldMatrix();
                const glm::vec3 up = glm::matrix_get_up(world);
                const glm::vec3 forward = glm::matrix_get_forward(world);
                const glm::vec3 position = glm::matrix_get_position(world)
                    + (up * HEIGHT_OFFSET) + (forward * FORWARD_OFFSET);
                glm::matrix_set_position(world, position);

                bullet->SetFiredPosition(position);
                bullet->SetWorld(world);

                const int bulletID = bullet->GetPhysicsID();
                const int tankID = tank.GetPhysicsIDs().Body;
                m_physics.SetFriction(bulletID, 0);
                m_physics.SetGravity(bulletID, 0.0f);
                m_physics.SetMotionState(bulletID, world);
                m_physics.AddToWorld(bulletID, true);
                m_physics.ResetVelocityAndForce(bulletID);
                m_physics.AddImpulse(forward * RECOIL_IMPULSE, glm::vec3(0, 0, 0), tankID);
                m_physics.AddImpulse(-forward * BULLET_IMPULSE, glm::vec3(0, 0, 0), bulletID);

                bullet->SetOwnerID(tankID);
                return;
            }
        }
    }
}
