////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Game.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "GameBuilder.h"
#include "GameData.h"
#include "BulletSpawner.h"
#include "MovementUpdater.h"
#include "SceneData.h"
#include "Tweaker.h"
#include "Camera.h"

Game::Game(Camera& camera) :
    m_camera(camera),
    m_data(std::make_unique<GameData>()),
    m_builder(std::make_unique<GameBuilder>())
{
}

Game::~Game() = default;

void Game::Tick(float deltatime)
{
    m_movement->Tick(deltatime);
    m_spawner->Tick(deltatime);

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

    // Post tick to reset requests
    m_data->player->ResetMovementRequest();
    for (auto& enemy : m_data->enemies)
    {
        enemy->ResetMovementRequest();
    } 
}

bool Game::Initialise(SceneData& data, PhysicsEngine& physics)
{
    m_movement = std::make_unique<MovementUpdater>(
        physics, *m_data, data);

    m_spawner = std::make_unique<BulletSpawner>(
        physics, *m_data, data);

    return Reset(data, physics);
}

bool Game::Reset(SceneData& data, PhysicsEngine& physics)
{
    return m_builder->Initialise(*m_data, data, physics);
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

void Game::FlipPlayer()
{
    m_data->player->Flip();
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