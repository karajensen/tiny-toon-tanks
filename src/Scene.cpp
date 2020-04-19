////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Scene.cpp
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

void Scene::Tick()
{
    for (auto& mesh : m_data->meshes)
    {
        mesh->Tick();
    }
}

bool Scene::Initialise(PhysicsEngine& physics)
{
    SceneBuilder builder;
    if (!builder.Initialise(*m_data, physics))
    {
        return false;
    }

    LogInfo("Scene: Successfully Initialised");
    return true;
}

void Scene::AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset)
{
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

SceneData& Scene::GetSceneData()
{
    return *m_data;
}