////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Gui.h"
#include "Common.h"
#include "Tweaker.h"
#include "Input.h"
#include "Camera.h"
#include "Timer.h"
#include "Scene.h"
#include "Glcommon.h"
#include <sstream>

Gui::Gui(Scene& scene,
         Camera& camera,
         Input& input,
         Timer& timer) :

    m_camera(camera),
    m_scene(scene),
    m_timer(timer)
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string barname = "GraphicsTweaker";
    m_tweakbar = TwNewBar(barname.c_str());
    m_tweaker = std::make_unique<Tweaker>(m_tweakbar);
    
    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Graphics Tweaker' " 
        << "position='" << border << " " << border << "' "
        << "size='250 " << WINDOW_HEIGHT-border*2 << "' "
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());

    FillTweakBar();

    auto AddKeyCallback = [&input](unsigned int key, unsigned int code)
    {
        input.AddCallback(key, false, [code]()
        { 
            TwKeyPressed(code, 0);
        });
    };

    // Keys required for modifying entries in the tweak bar
    AddKeyCallback(GLFW_KEY_0, '0');
    AddKeyCallback(GLFW_KEY_1, '1');
    AddKeyCallback(GLFW_KEY_2, '2');
    AddKeyCallback(GLFW_KEY_3, '3');
    AddKeyCallback(GLFW_KEY_4, '4');
    AddKeyCallback(GLFW_KEY_5, '5');
    AddKeyCallback(GLFW_KEY_6, '6');
    AddKeyCallback(GLFW_KEY_7, '7');
    AddKeyCallback(GLFW_KEY_8, '8');
    AddKeyCallback(GLFW_KEY_9, '9');
    AddKeyCallback(GLFW_KEY_PERIOD, '.');
    AddKeyCallback(GLFW_KEY_MINUS, '-');
    AddKeyCallback(GLFW_KEY_ENTER, TW_KEY_RETURN);
    AddKeyCallback(GLFW_KEY_LEFT, TW_KEY_LEFT);
    AddKeyCallback(GLFW_KEY_RIGHT, TW_KEY_RIGHT);
    AddKeyCallback(GLFW_KEY_TAB, TW_KEY_TAB);
    AddKeyCallback(GLFW_KEY_END, TW_KEY_END);
    AddKeyCallback(GLFW_KEY_HOME, TW_KEY_HOME);
    AddKeyCallback(GLFW_KEY_BACKSPACE, TW_KEY_BACKSPACE);

    LogInfo("GUI: Tweak bar initialised");
}

Gui::~Gui()
{
    if(m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Gui::Toggle()
{
    m_show = !m_show;
}

void Gui::Render()
{
    if (m_show)
    {
        TwDraw();
    }
}

void Gui::Update(const Input& input)
{
    if (m_show)
    {
        m_tweaker->Update();

        if (input.IsMouseMovedThisTick())
        {
            TwMouseMotion(input.GetMouseX(), input.GetMouseY());
        }
        else if (input.IsLeftMousePressedThisTick())
        {
            TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
        }
        else if (input.IsLeftMouseReleasedThisTick())
        {
            TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
        }
    }
}

void Gui::FillTweakBar()
{
    TwRemoveAllVars(m_tweakbar);
    m_tweaker->ClearEntries();
    m_scene.AddToTweaker(*m_tweaker, [this](){ FillTweakBar(); });
    m_camera.AddToTweaker(*m_tweaker);
    m_timer.AddToTweaker(*m_tweaker);
}