////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Application.h"
#include "OpenGL.h"
#include "BulletPhysics.h"
#include "SoundEngine.h"
#include "Input.h"
#include "Camera.h"
#include "Gui.h"
#include "Timer.h"
#include "Common.h"
#include "Glcommon.h"
#include "Scene.h"

Application::Application() :
    m_sound(std::make_unique<SoundEngine>()),
    m_camera(std::make_unique<Camera>()),
    m_scene(std::make_unique<Scene>()),
    m_timer(std::make_unique<Timer>()),
    m_physics(std::make_unique<BulletPhysicsWorld>())
{
}

Application::~Application() = default;

void Application::Run()
{
    m_sound->PlayMusic(SoundEngine::GAME);

    while(m_engine->IsRunning())
    {
        m_timer->StartSection(Timer::SCENE);

        m_timer->UpdateTimer();
        const float deltaTime = m_timer->GetDeltaTime();

        m_sound->FadeMusic();
        m_input->Update();       
        m_gui->Update(*m_input);
        m_scene->Tick(deltaTime);

        m_camera->Update(m_input->IsRightMouseDown(), 
            m_input->GetMouseDirection(), deltaTime);

        m_timer->StopSection(Timer::SCENE);
        m_timer->StartSection(Timer::PHYSICS);

        m_physics->Tick(0.01f);

        m_timer->StopSection(Timer::PHYSICS);
        m_timer->StartSection(Timer::RENDERING);

        m_engine->RenderScene();
        m_gui->Render();
        m_engine->EndRender();

        m_timer->StopSection(Timer::RENDERING);
    }
}

void Application::Release()
{
    // Release all openGL resources before terminating engine
    m_gui.reset();
    m_scene.reset();

    m_engine.reset();
}

bool Application::Initialise()
{
    m_engine = std::make_unique<OpenGL>(
        m_scene->GetSceneData(), *m_camera);

    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    if (!m_scene->Initialise(*m_physics))
    {
        LogError("Could not initialise scene");
        return false;
    }

    InitialiseInput();

    // Requires application to be fully initialiseds
    m_gui = std::make_unique<Gui>(
        *m_scene, *m_camera, *m_input, *m_timer);

    return true;
}

void Application::InitialiseInput()
{
    m_input = std::make_unique<Input>(m_engine->GetWindow());

    m_input->AddCallback(GLFW_KEY_F1, false, 
        [this](){ m_gui->Toggle(); });

    m_input->AddCallback(GLFW_KEY_F2, true, 
        [this](){ m_scene->SetPostMap(PostProcessing::FINAL_MAP); });

    m_input->AddCallback(GLFW_KEY_F3, true, 
        [this](){ m_scene->SetPostMap(PostProcessing::SCENE_MAP); });

    m_input->AddCallback(GLFW_KEY_F4, true, 
        [this](){ m_scene->SetPostMap(PostProcessing::NORMAL_MAP); });

    m_input->AddCallback(GLFW_KEY_F5, true, 
        [this](){ m_scene->SetPostMap(PostProcessing::TOONLINE_MAP); });

    m_input->AddCallback(GLFW_KEY_W, true, 
        [this](){ m_camera->Forward(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_S, true, 
        [this](){ m_camera->Forward(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_D, true, 
        [this](){ m_camera->Right(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_A, true, 
        [this](){ m_camera->Right(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_Q, true,
        [this](){ m_camera->Up(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_E, true, 
        [this](){ m_camera->Up(m_timer->GetDeltaTime()); });
}