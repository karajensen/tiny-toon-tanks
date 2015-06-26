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
#include "common.h"

Application::Application() :
    m_sound(std::make_unique<SoundEngine>()),
    m_camera(std::make_unique<Camera>()),
    m_engine(std::make_unique<OpenGL>(*m_camera)),
    m_timer(std::make_unique<Timer>())
{
}

Application::~Application() = default;

void Application::Run()
{
    m_sound->PlayMusic(SoundEngine::GAME);

    while(m_engine->IsRunning())
    {
        m_sound->FadeMusic();

        m_timer->StartSection(Timer::SCENE_UPDATE);

        m_timer->UpdateTimer();
        const float deltaTime = m_timer->GetDeltaTime();

        m_input->Update();       
        m_gui->Update(*m_input);

        m_camera->Update(deltaTime);

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
    m_engine.reset();
}

bool Application::Initialise()
{
    if (!m_engine->Initialise())
    {
        LogError("Could not initialise render engine");
        return false;
    }

    InitialiseInput();

    // Requires application to be fully initialises
    m_gui = std::make_unique<Gui>(*m_camera, *m_input, *m_timer,
        std::bind(&OpenGL::ToggleWireframe, m_engine.get()));

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
   
}