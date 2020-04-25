////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <functional>

struct CTwBar;
class Input;
class Tweaker;
class Timer;
class Camera;
class Scene;
class Game;

/**
* Opens up a tweak bar for manipulating the scene
*/
class Gui
{
public:

    /**
    * Constructor
    * @param scene Holds and managers scene data
    * @param game Holds and managers game data
    * @param camera Allows modifying the camera
    * @param input Allows adding key callbacks
    * @param timer Allows viewing the application times
    */
    Gui(Scene& scene,
        Game& game,
        Camera& camera, 
        Input& input,
        Timer& timer);

    /**
    * Destructor
    */
    ~Gui();

    /**
    * Renders the tweak bar
    */
    void Render();

    /**
    * Updates the input to the tweak bar
    */
    void Update(const Input& input);

    /**
    * Toggles the tweak bar
    */
    void Toggle();

private:

    /**
    * Prevent copying
    */
    Gui(const Gui&) = delete;
    Gui& operator=(const Gui&) = delete;

    /**
    * Refills the tweak bar
    */
    void FillTweakBar();

private:

    Game& m_game;                          ///< Holds and manages game data
    Scene& m_scene;                        ///< Holds and manages scene data
    Camera& m_camera;                      ///< Allows modifying the view
    Timer& m_timer;                        ///< Allows viewing the application times
    CTwBar* m_tweakbar = nullptr;          ///< Tweak bar for manipulating the scene
    bool m_show = false;                   ///< Whether the GUI is displayed
    std::unique_ptr<Tweaker> m_tweaker;    ///< Helper for modifying the tweak bar
};                     