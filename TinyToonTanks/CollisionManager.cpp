////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"
#include "SoundEngine.h"

namespace
{
    const float GUN_PIECE_SPAWN_OFFSET = 2.0f;
    const int BULLET_FULL_DAMAGE = 2;
    const int BULLET_HALF_DAMAGE = 1;
}

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
        else if (collisionEvent->Resolved)
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
    const auto meshA = collisionEvent->BodyA.MeshID;
    const auto meshB = collisionEvent->BodyB.MeshID;

    if (meshA == MeshID::BULLET || meshB == MeshID::BULLET)
    {
        BulletCollisionLogic(collisionEvent);
    }
    else if (meshA == MeshID::TANK || meshA == MeshID::TANKGUN || 
        meshB == MeshID::TANK || meshB == MeshID::TANKGUN)
    {
        TankCollisionLogic(collisionEvent);
    }

    collisionEvent->Processed = true;
}

void CollisionManager::TankCollisionLogic(const CollisionEvent* collisionEvent)
{
    const auto meshA = collisionEvent->BodyA.MeshID;
    const auto meshB = collisionEvent->BodyB.MeshID;
    const auto instanceA = collisionEvent->BodyA.MeshInstance;
    const auto instanceB = collisionEvent->BodyB.MeshInstance;

    if ((meshA == MeshID::TANK || meshA == MeshID::TANKGUN) && meshB == MeshID::GROUND)
    {
        auto tank = GetTank(instanceA);
        if (tank->IsAlive())
        {
            tank->SetDropping(false);
        }
    }
    else if ((meshB == MeshID::TANK || meshB == MeshID::TANKGUN) && meshA == MeshID::GROUND)
    {
        auto tank = GetTank(instanceB);
        if (tank->IsAlive())
        {
            tank->SetDropping(false);
        }
    }

    if (meshA == MeshID::TANK && meshB == MeshID::TANK)
    {
        SoundEngine::PlaySoundEffect(SoundEngine::BANG);
    }
    else if ((meshA == MeshID::TANK && meshB == MeshID::WALL) ||
        (meshB == MeshID::TANK && meshA == MeshID::WALL))
    {
        SoundEngine::PlaySoundEffect(SoundEngine::WALLBANG);
    }
}

glm::vec3 CollisionManager::GetBulletDirectionFromWall(const glm::vec3& direction, int wall)
{
    // v' = v - 2(v.n)n
    const auto& normal = m_gameData.wallNormals[wall];
    return glm::normalize(direction - (2.0f * glm::dot(normal, direction) * normal));
}

void CollisionManager::BulletCollisionLogic(const CollisionEvent* collisionEvent)
{
    const auto meshA = collisionEvent->BodyA.MeshID;
    const auto meshB = collisionEvent->BodyB.MeshID;
    const auto instanceA = collisionEvent->BodyA.MeshInstance;
    const auto instanceB = collisionEvent->BodyB.MeshInstance;
    
    auto bulletA = meshA == MeshID::BULLET ? m_gameData.bullets[instanceA].get() : nullptr;
    auto bulletB = meshB == MeshID::BULLET ? m_gameData.bullets[instanceB].get() : nullptr;

    // Bullet-wall collision
    if (meshA == MeshID::WALL || meshB == MeshID::WALL)
    {
        if (bulletA)
        {
            bulletA->TakeDamage(BULLET_HALF_DAMAGE);
            bulletA->SetGenerateImpulse(true,
                GetBulletDirectionFromWall(m_physics.GetVelocity(bulletA->GetPhysicsID()), instanceB));
        }

        if (bulletB)
        {
            bulletB->TakeDamage(BULLET_HALF_DAMAGE);
            bulletB->SetGenerateImpulse(true,
                GetBulletDirectionFromWall(m_physics.GetVelocity(bulletB->GetPhysicsID()), instanceA));
        }
    }
    // Bullet-tank collision
    else if (meshA == MeshID::TANK || meshA == MeshID::TANKGUN
        || meshB == MeshID::TANK || meshB == MeshID::TANKGUN)
    {
        auto* tank = (meshA == MeshID::TANK || meshA == MeshID::TANKGUN) ?
            GetTank(instanceA) : GetTank(instanceB);

        if (tank->IsAlive())
        {
            auto bullet = bulletA ? bulletA : bulletB;
            if (tank->GetPhysicsIDs().Body == bullet->GetOwnerID())
            {
                return; // No friendly fire
            }

            bullet->TakeDamage(BULLET_FULL_DAMAGE);
            tank->TakeDamage(BULLET_HALF_DAMAGE);

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
                const auto position = glm::matrix_get_position(world);
                const auto forward = -glm::normalize(glm::matrix_get_forward(world));
                glm::matrix_set_position(world, position + (forward * GUN_PIECE_SPAWN_OFFSET));
                tank->SetPieceWorldMatrix(MeshID::TANKP4, world);
                m_physics.SetMotionState(ids.P4, world);
                m_physics.AddToWorld(ids.P4, true);

                tank->SetIsAlive(false);
            }
        }
    }
    // Everything else destroys bullet
    else
    {
        if (bulletA)
        {
            bulletA->TakeDamage(BULLET_FULL_DAMAGE);
        }

        if (bulletB)
        {
            bulletB->TakeDamage(BULLET_FULL_DAMAGE);
        }
    }

    bool bulletDestroyed = false;

    if (bulletA && bulletA->Health() <= 0)
    {
        bulletDestroyed = true;
        bulletA->SetIsAlive(false);
        m_physics.AddToWorld(bulletA->GetPhysicsID(), false);
        m_physics.ResetVelocityAndForce(bulletA->GetPhysicsID());
        m_gameData.toonText->Show(bulletA->GetPosition());
    }

    if (bulletB && bulletB->Health() <= 0)
    {
        bulletDestroyed = true;
        bulletB->SetIsAlive(false);
        m_physics.AddToWorld(bulletB->GetPhysicsID(), false);
        m_physics.ResetVelocityAndForce(bulletB->GetPhysicsID());
        m_gameData.toonText->Show(bulletB->GetPosition());
    }

    if (bulletDestroyed)
    {
        SoundEngine::PlaySoundEffect(SoundEngine::EXPLODE);
    }
}