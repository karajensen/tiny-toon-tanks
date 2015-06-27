////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Application.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Application.h"
#include "OpenGL.h"
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
    m_engine(std::make_unique<OpenGL>(*m_camera)),
    m_timer(std::make_unique<Timer>()),
    m_scene(std::make_unique<Scene>())
{
}

Application::~Application() = default;

void Application::Run()
{
    m_sound->PlayMusic(SoundEngine::GAME);

    while(m_engine->IsRunning())
    {
        m_timer->StartSection(Timer::SCENE_UPDATE);

        m_timer->UpdateTimer();
        const float deltaTime = m_timer->GetDeltaTime();

        m_sound->FadeMusic();
        m_input->Update();       
        m_gui->Update(*m_input);
        m_camera->Update(deltaTime);
        m_scene->Tick(deltaTime);

        m_timer->StopSection(Timer::SCENE_UPDATE);
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
    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    if (!m_scene->Initialise())
    {
        LogError("Could not initialise scene");
        return false;
    }

    InitialiseInput();

    // Requires application to be fully initialiseds
    m_gui = std::make_unique<Gui>(*m_scene, *m_camera, *m_input, *m_timer);

    return true;
}

void Application::InitialiseInput()
{
    m_input = std::make_unique<Input>(m_engine->GetWindow());

    m_input->AddCallback(GLFW_KEY_W, true, 
        [this](){ m_camera->Forward(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_S, true, 
        [this](){ m_camera->Forward(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_D, true, 
        [this](){ m_camera->Right(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_A, true, 
        [this](){ m_camera->Right(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_Q, true,
        [this](){ m_camera->Up(m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_E, true, 
        [this](){ m_camera->Up(-m_timer->GetDeltaTime()); });

    m_input->AddCallback(GLFW_KEY_F1, false, 
        [this](){ m_gui->Toggle(); });
}