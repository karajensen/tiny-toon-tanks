////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Game.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "GameBuilder.h"
#include "GameData.h"
#include "SceneData.h"
#include "Tweaker.h"

Game::Game() :
    m_data(std::make_unique<GameData>())
{
}

Game::~Game() = default;

void Game::Tick(float deltatime)
{
}

bool Game::Initialise(SceneData& data, BulletPhysicsWorld& physics)
{
    GameBuilder builder;
    return builder.Initialise(*m_data, data, physics);
}

void Game::Reload()
{
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
