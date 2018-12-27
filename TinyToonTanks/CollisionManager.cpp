////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionManager.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "CollisionManager.h"
#include "PhysicsEngine.h"
#include "GameData.h"
#include "SceneData.h"
#include "DataIDs.h"

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
                    previouslyGenerated = true;
                    break;
                }
            }

            // If already in the list, keep current and continue to next event
            if (previouslyGenerated)
            {
                m_events[i].Resolved = false;
                continue;
            }

            // A new event, add to the list
            m_events.push_back(collisionEvent);
        }
    }
}

void CollisionManager::CollisionResolution()
{
}