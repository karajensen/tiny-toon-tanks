////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "SceneBuilder.h"
#include "SceneData.h"
#include "Tweaker.h"

Scene::Scene() :
    m_data(std::make_unique<SceneData>())
{
}

Scene::~Scene() = default;

void Scene::Tick(float deltatime)
{
}

bool Scene::Initialise()
{
    m_builder = std::make_unique<SceneBuilder>(*m_data);
    return m_builder->Initialise();
}

void Scene::Reload()
{
}

void Scene::AddToTweaker(Tweaker& tweaker)
{
}