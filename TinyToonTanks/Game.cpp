////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Game.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "GameBuilder.h"
#include "GameData.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "TankManager.h"
#include "SceneData.h"
#include "Tweaker.h"
#include "Camera.h"

Game::Game(Camera& camera, PhysicsEngine& physicsEngine) :
    m_camera(camera),
    m_physicsEngine(physicsEngine),
    m_data(std::make_unique<GameData>()),
    m_builder(std::make_unique<GameBuilder>())
{
}

Game::~Game() = default;

void Game::PrePhysicsTick(float deltatime)
{
    m_data->player->Update(deltatime);
    for (auto& enemy : m_data->enemies)
    {
        enemy->Update(deltatime);
    } 

    m_tankManager->PrePhysicsTick(deltatime);
    m_bulletManager->PrePhysicsTick();

    // Fix the camera to the player
    if (!m_camera.IsFlyCamera())
    {
        const float cameraOffset = 20.0f;
        const glm::vec3 tankPosition = m_data->player->GetPosition();
        const glm::vec3 cameraPosition(
            tankPosition.x - cameraOffset,
            tankPosition.y + cameraOffset,
            tankPosition.z - cameraOffset);

        m_camera.SetPosition(cameraPosition);
        m_camera.SetTarget(tankPosition);
    }

    // Reset movement requests
    m_data->player->ResetMovementRequest();
    for (auto& enemy : m_data->enemies)
    {
        enemy->ResetMovementRequest();
    } 
}

void Game::PostPhysicsTick(float deltatime)
{
    m_tankManager->PostPhysicsTick();
    m_bulletManager->PostPhysicsTick();

    m_collisionManager->CollisionDetection();
    m_collisionManager->CollisionResolution();

    // Do after collision resolution as it will enable/disable instances
    m_data->toonText->Tick(m_camera, deltatime);
}

bool Game::Initialise(SceneData& data)
{
    m_tankManager = std::make_unique<TankManager>(
        m_physicsEngine, *m_data, data);

    m_bulletManager = std::make_unique<BulletManager>(
        m_physicsEngine, *m_data, data);

    m_collisionManager = std::make_unique<CollisionManager>(
        m_physicsEngine, *m_data, data);

    return Reset(data);
}

bool Game::Reset(SceneData& data)
{
    return m_builder->Initialise(*m_data, data, m_physicsEngine, *m_collisionManager);
}

void Game::AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset)
{
    tweaker.SetGroup("Player");
    m_data->player->AddToTweaker(tweaker);

    tweaker.SetGroup("Enemy");
    tweaker.AddIntEntry("Selected Enemy", 
        [this](){ return m_selectedEnemy; }, 
        [this, reset](const int value){ m_selectedEnemy = value; reset(); }, 
        m_data->enemies.size()-1);
    m_data->enemies[m_selectedEnemy]->AddToTweaker(tweaker);
}

void Game::FirePlayer()
{
    m_data->player->Fire();
}

void Game::RotatePlayer(bool left)
{
    m_data->player->Rotate(left);
}

void Game::RotatePlayerGun(bool left)
{
    m_data->player->RotateGun(left);
}

void Game::MovePlayer(bool forwards)
{
    m_data->player->Move(forwards);
}