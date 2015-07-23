////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Scene.h"
#include "SceneBuilder.h"
#include "SceneData.h"
#include "Tweaker.h"
#include "Game.h"
#include "Camera.h"

Scene::Scene(Camera& camera) :
    m_camera(camera),
    m_data(std::make_unique<SceneData>()),
    m_game(std::make_unique<Game>())
{
}

Scene::~Scene() = default;

void Scene::Tick(float deltatime)
{
    for (auto& mesh : m_data->meshes)
    {
        mesh->Tick();
    }

    m_game->Tick(deltatime);

    if (!m_camera.IsFlyCamera())
    {
        const float cameraOffset = 20.0f;
        const glm::vec3 tankPosition = m_game->GetFocusTankPosition();
        const glm::vec3 cameraPosition(
            tankPosition.x - cameraOffset,
            tankPosition.y + cameraOffset,
            tankPosition.z - cameraOffset);

        m_camera.SetPosition(cameraPosition);
        m_camera.SetTarget(tankPosition);
    }
}

bool Scene::Initialise(BulletPhysicsWorld& physics)
{
    SceneBuilder builder;
    if (!builder.Initialise(*m_data, physics))
    {
        return false;
    }

    if (!m_game->Initialise(*m_data, physics))
    {
        return false;
    }

    LogInfo("Scene: Successfully Initialised");
    return true;
}

void Scene::Reload()
{
    m_game->Reload();
}

void Scene::AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset)
{
    m_game->AddToTweaker(tweaker, reset);

    tweaker.SetGroup("PostProcessing");
    m_data->post->AddToTweaker(tweaker);

    auto AddElement = [this, &tweaker, reset](std::string group, std::string name, 
                                              int& selected, unsigned int size)
    {
        tweaker.SetGroup(group);
        tweaker.AddIntEntry(name, 
            [this, &selected](){ return selected; }, 
            [this, &selected, reset](const int value){ selected = value; reset(); }, 
            size-1);
    };

    AddElement("Lights", "Selected Light", m_selectedLight, m_data->lights.size());
    m_data->lights[m_selectedLight]->AddToTweaker(tweaker);

    AddElement("Meshes", "Selected Mesh", m_selectedMesh, m_data->meshes.size());
    m_data->meshes[m_selectedMesh]->AddToTweaker(tweaker);

    AddElement("Hulls", "Selected Hull", m_selectedHull, m_data->hulls.size());
    m_data->hulls[m_selectedHull]->AddToTweaker(tweaker);
}

void Scene::SetPostMap(PostProcessing::Map map)
{
    m_data->post->SetPostMap(map);
}

const SceneData& Scene::GetSceneData() const
{
    return *m_data;
}