////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

class SoundEngine;
class OpenGL;
class Camera;
class Input;
class Timer;
class Gui;
class Scene;

/**
* Main application class
*/
class Application
{
public:

    Application();
    ~Application();

    /**
    * Main loop for the application
    */
    void Run();

    /**
    * Initialise the world
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * Releases the world
    */
    void Release();

private: 

    /**
    * Prevent copying
    */
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
    * Initialises the input for the application
    */
    void InitialiseInput();

    std::unique_ptr<SoundEngine> m_sound;  ///< Controller for sound
    std::unique_ptr<Camera> m_camera;      ///< The camera viewing the scene
    std::unique_ptr<Input> m_input;        ///< Manages input from the window
    std::unique_ptr<OpenGL> m_engine;      ///< Render engine to use for application
    std::unique_ptr<Gui> m_gui;            ///< Manipulates the scene
    std::unique_ptr<Timer> m_timer;        ///< Keeps track of time between ticks and simulation
    std::unique_ptr<Scene> m_scene;        ///< Manager of game objects
};