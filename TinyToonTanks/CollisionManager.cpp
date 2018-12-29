////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"
#include "SoundEngine.h"
#include "GlmHelper.h"

CollisionManager::CollisionManager(PhysicsEngine& physics,
                                   GameData& gameData,
                                   SceneData& sceneData)
    : m_physics(physics)
    , m_gameData(gameData)
    , m_sceneData(sceneData)
{
}

void CollisionManager::CollisionDetection()
{
    const int collisionAmount = m_physics.GetCollisionAmount();
    CollisionEvent collisionEvent;

    for (int i = 0; i < collisionAmount; ++i)
    {
        if (m_physics.GenerateCollisionEvent(i, collisionEvent))
        {
            // Check if collision event has previously generated
            bool previouslyGenerated = false;
            for (size_t j = 0; j < m_events.size(); ++j)
            {
                if (collisionEvent == m_events[j])
                {
                    m_events[j].Resolved = false;
                    previouslyGenerated = true;
                    break;
                }
            }

            // If already in the list, keep current and continue to next event
            if (previouslyGenerated)
            {
                continue;
            }

            // A new event, add to the list
            m_events.push_back(collisionEvent);
        }
    }
}

void CollisionManager::CollisionResolution()
{
    // Check if collision event needs to be processed or removed
    for (size_t i = 0; i < m_events.size(); i++)
    {
        CollisionEvent* collisionEvent = &m_events[i];
        if (!collisionEvent->Processed)
        {
            ResolveCollisionEvent(collisionEvent);
        }
        else if (collisionEvent->Resolved || collisionEvent->Ignore)
        {
            // Remove event if no longer happening
            m_events.erase(m_events.begin() + i);
        }
    }

    // Reset all remaining messages to not colliding for next frame
    for (size_t i = 0; i < m_events.size(); ++i)
    {
        m_events[i].Resolved = true;
    }
}

int CollisionManager::GetCollisionGroupIndex() const
{
    return m_collisionGroupIndex;
}

void CollisionManager::IncrementCollisionGroupIndex()
{
    ++m_collisionGroupIndex;
}

Tank* CollisionManager::GetTank(int instanceID) const
{
    if (instanceID == Instance::PLAYER)
    {
        return m_gameData.player.get();
    }
    else
    {
        return m_gameData.enemies[instanceID].get();
    }
}

void CollisionManager::ResolveCollisionEvent(CollisionEvent* collisionEvent)
{
    const auto shapeA = collisionEvent->BodyA.MeshID;
    const auto shapeB = collisionEvent->BodyB.MeshID;
    const auto instanceA = collisionEvent->BodyA.MeshInstance;
    const auto instanceB = collisionEvent->BodyB.MeshInstance;

    if ((shapeA == MeshID::BULLET) || (shapeB == MeshID::BULLET))
    {
        BulletCollisionLogic(shapeA, shapeB, instanceA, instanceB, collisionEvent->Ignore);
        BulletCollisionLogic(shapeB, shapeA, instanceB, instanceA, collisionEvent->Ignore);
    }
    else if ((shapeA == MeshID::TANK) || (shapeA == MeshID::TANKGUN) || (shapeB == MeshID::TANK) || (shapeB == MeshID::TANKGUN))
    {
        TankCollisionLogic(shapeA, shapeB, instanceA);
        TankCollisionLogic(shapeB, shapeA, instanceB);

        if ((shapeA == MeshID::TANK) && (shapeB == MeshID::TANK))
        {
            SoundEngine::PlaySoundEffect(SoundEngine::BANG);
        }
        else if ((shapeA == MeshID::TANK) && (shapeB == MeshID::WALL) ||
            (shapeB == MeshID::TANK) && (shapeA == MeshID::WALL))
        {
            SoundEngine::PlaySoundEffect(SoundEngine::WALLBANG);
        }
    }

    collisionEvent->Processed = true;
}

void CollisionManager::TankCollisionLogic(int shape, int shapeHit, int instance)
{
    if (((shape == MeshID::TANK) || (shape == MeshID::TANKGUN)) && (shapeHit == MeshID::GROUND))
    {
        auto tank = GetTank(instance);
        if (tank->IsAlive())
        {
            tank->SetDropping(false);
        }
    }
}

void CollisionManager::BulletCollisionLogic(int shape, int shapeHit, int instance, int instanceHit, bool& ignoreEvent)
{
    // Collision between tank and own bullet
    {
        Bullet* bullet = nullptr;
        if (shape == MeshID::BULLET)
        {
            bullet = m_gameData.bullets[instance].get();
        }
        else
        {
            bullet = m_gameData.bullets[instanceHit].get();
        }

        // Get tank if colliding object is a tank
        Tank* tank = nullptr;
        if ((shape == MeshID::TANK) || (shape == MeshID::TANKGUN))
        {
            tank = GetTank(instance);
        }
        else if ((shapeHit == MeshID::TANK) || (shapeHit == MeshID::TANKGUN))
        {
            tank = GetTank(instanceHit);
        }

        if (!bullet->AllowScore())
        {
            if (tank != nullptr)
            {
                // If the bullet isn't colliding with the tank that shot it
                const auto tankGroup = m_physics.GetGroup(tank->GetPhysicsIDs().Body);
                const auto bulletGroup = m_physics.GetGroup(bullet->GetPhysicsID());
                if (tankGroup != bulletGroup)
                {
                    bullet->SetAllowScore(true);
                }
                else
                {
                    ignoreEvent = true;
                    return;
                }
            }
            else
            {
                bullet->SetAllowScore(true);
            }

            // if now allowing score on bullet, change the bullet's group
            if (bullet->AllowScore())
            {
                m_physics.SetGroup(bullet->GetPhysicsID(), GetCollisionGroupIndex());
                IncrementCollisionGroupIndex();
            }
        }
    }

    // Collision between bullet and bullet
    if (shape == MeshID::BULLET)
    {
        auto* bullet = m_gameData.bullets[instance].get();
        if (!bullet->IsAlive())
        { 
            return; // No need to go further
        }

        // If hitting a wall, bounce the bullet
        if (shapeHit == MeshID::WALL)
        {
            // Bullet hits a wall
            bullet->TakeDamage(1);

            // Add impulse in new direction
            bullet->SetGenerateImpulse(true);
        }
        else
        {
            // If hitting anything else, take damage
            auto* bulletHit = m_gameData.bullets[instanceHit].get();
            bullet->TakeDamage(bulletHit->DamageDealt());
        }

        // Check if bullet still alive
        if (bullet->Health() <= 0)
        {
            SoundEngine::PlaySoundEffect(SoundEngine::EXPLODE);

            const int ID = bullet->GetPhysicsID();
            bullet->SetIsAlive(false);
            m_physics.AddToWorld(ID, false);
            m_physics.ResetVelocityAndForce(ID);
        }
    }
    // Collision between bullet and enemy
    else if (shape == MeshID::TANK || shape == MeshID::TANKGUN)
    {
        // Tank takes damage
        auto* tank = GetTank(instance);
        if (!tank->IsAlive()) 
        { 
            return; // No need to go further
        }

        auto bullet = m_gameData.bullets[instanceHit].get();
        tank->TakeDamage(bullet->DamageDealt());

        if (tank->Health() <= 0)
        {
            const auto& ids = tank->GetPhysicsIDs();
            auto world = tank->GetWorldMatrix();

            m_physics.AddToWorld(ids.Body, false);
            m_physics.AddToWorld(ids.Gun, false);

            tank->SetPieceWorldMatrix(MeshID::TANKP1, world);
            m_physics.SetMotionState(ids.P1, world);
            m_physics.AddToWorld(ids.P1, true);

            tank->SetPieceWorldMatrix(MeshID::TANKP2, world);
            m_physics.SetMotionState(ids.P2, world);
            m_physics.AddToWorld(ids.P2, true);

            tank->SetPieceWorldMatrix(MeshID::TANKP3, world);
            m_physics.SetMotionState(ids.P3, world);
            m_physics.AddToWorld(ids.P3, true);

            // Offset the gun slightly so it doesn't balance on the tank top
            const auto gunPieceOffset = 2.0f;
            const auto position = glm::matrix_get_position(world);
            const auto forward = -glm::normalize(glm::matrix_get_forward(world));
            glm::matrix_set_position(world, position + (forward * gunPieceOffset));
            tank->SetPieceWorldMatrix(MeshID::TANKP4, world);
            m_physics.SetMotionState(ids.P4, world);
            m_physics.AddToWorld(ids.P4, true);

            tank->SetIsAlive(false);
        }
    }
}