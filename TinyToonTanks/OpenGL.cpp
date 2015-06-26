////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - OpenGL.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl.h"
#include "glcommon.h"
#include "camera.h"
#include "Quad.h"
#include "Rendertarget.h"

OpenGL::OpenGL(const Camera& camera) :
    m_camera(camera),
    m_quad(std::make_unique<Quad>("PostQuad")),
    m_backBuffer(std::make_unique<RenderTarget>("BackBuffer")),
    m_sceneTarget(std::make_unique<RenderTarget>("SceneTarget", false))
{
}

OpenGL::~OpenGL()
{
    Release();
}

void OpenGL::Release()
{
    // All resources must be destroyed before the engine
    m_quad.reset();
    m_sceneTarget.reset();
    m_backBuffer.reset();

    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

bool OpenGL::IsRunning() const
{
    return !glfwWindowShouldClose(m_window) && 
          glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

bool OpenGL::Initialise()
{
    if (!glfwInit())
    {
        LogError("OpenGL: Could not initialize GLFW");
        return false;
    }

    m_window = glfwCreateWindow(WINDOW_WIDTH, 
        WINDOW_HEIGHT, "Tiny Toon Tanks", nullptr, nullptr);

    if (!m_window)
    {
        LogError("OpenGL: Could not create window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) 
    {
        LogError("OpenGL: OGL Load Failed");
        return false;
    }

    glClearColor(0.22f, 0.49f, 0.85f, 0.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glFrontFace(GL_CCW); 
    glDisablei(GL_BLEND, 0);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    if (!m_backBuffer->Initialise() ||
        !m_sceneTarget->Initialise())
    {
        LogError("OpenGL: Failed to initialise render targets");
        return false;
    }

    if (!m_quad->Initialise())
    {
        LogError("OpenGL: Failed to initialise quad");
        return false;
    }

    if (HasCallFailed())
    {
        LogError("OpenGL: Failed to initialise scene");
        return false;
    }

    LogInfo("OpenGL: Initialised " + std::to_string(ogl_GetMajorVersion()) +
        "." + std::to_string(ogl_GetMinorVersion()));

    return true;
}

GLFWwindow& OpenGL::GetWindow() const
{
    assert(m_window);
    return *m_window;
}

void OpenGL::RenderScene()
{
    m_backBuffer->SetActive();
}

void OpenGL::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGL::EnableAlphaBlending(bool enable, bool multiply)
{
    if (enable != m_isAlphaBlend)
    {
        m_isAlphaBlend = enable;
        enable ? glEnablei(GL_BLEND, 0) : glDisablei(GL_BLEND, 0);
        enable ? glEnablei(GL_BLEND, 1) : glDisablei(GL_BLEND, 1);
    }
    if (multiply != m_isBlendMultiply)
    {
        m_isBlendMultiply = multiply;
        if (multiply)
        {
            glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_DST_ALPHA, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
        else
        {
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        }
    }
}

void OpenGL::EnableBackfaceCull(bool enable)
{
    if(enable != m_isBackfaceCull)
    {
        m_isBackfaceCull = enable;
        enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
}

void OpenGL::EnableDepthWrite(bool enable)
{
    if (enable != m_isDepthWrite)
    {
        m_isDepthWrite = enable;
        enable ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
    }
}

void OpenGL::ToggleWireframe()
{
    m_isWireframe = !m_isWireframe;
}