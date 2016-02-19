////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - OpenGL.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "OpenGL.h"
#include "Common.h"
#include "Glcommon.h"
#include "Camera.h"
#include "Quad.h"
#include "SceneData.h"
#include "Rendertarget.h"

namespace
{
    const int NO_INDEX = -1;
}

OpenGL::OpenGL(const SceneData& scene, const Camera& camera) :
    m_camera(camera),
    m_scene(scene),
    m_quad(std::make_unique<Quad>("PostQuad"))
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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glDisablei(GL_BLEND, 0);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    m_backBuffer = std::make_unique<RenderTarget>("BackBuffer");
    m_sceneTarget = std::make_unique<RenderTarget>("Scene", SCENE_TEXTURES, true);

    if (!m_backBuffer->Initialise() || !m_sceneTarget->Initialise())
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
    m_sceneTarget->SetActive();
    RenderMeshes();

    m_backBuffer->SetActive();
    RenderPostProcessing();
}

void OpenGL::RenderPostProcessing()
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false, false);   

    SetSelectedShader(ShaderID::POST);
    auto& shader = *m_scene.shaders[m_selectedShader];
    auto& post = *m_scene.post;

    shader.SendUniform("finalMask", post.Mask(PostProcessing::FINAL_MAP));
    shader.SendUniform("sceneMask", post.Mask(PostProcessing::SCENE_MAP));
    shader.SendUniform("normalMask", post.Mask(PostProcessing::NORMAL_MAP));
    shader.SendUniform("toonlineMask", post.Mask(PostProcessing::TOONLINE_MAP));

    shader.SendTexture("SceneSampler", *m_sceneTarget, ID_COLOUR);
    shader.SendTexture("NormalSampler", *m_sceneTarget, ID_NORMAL);

    m_quad->PreRender();
    EnableSelectedShader();
    m_quad->Render();

    shader.ClearTexture("SceneSampler", *m_sceneTarget);
    shader.ClearTexture("NormalSampler", *m_sceneTarget);
}

void OpenGL::RenderMeshes()
{
    auto renderInstance = [this](const glm::mat4& world, int texture)
    {
        UpdateShader(world, texture); 
    };

    for (const auto& mesh : m_scene.meshes)
    {
        if (mesh->IsVisible() && UpdateShader(*mesh))
        {
            mesh->PreRender();
            EnableSelectedShader();
            mesh->RenderTextured(renderInstance);
        }
    }
}

void OpenGL::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGL::UpdateShader(const glm::mat4& world, int texture)
{
    UpdateShader(world);
    SendTexture("DiffuseSampler", texture);
}

void OpenGL::UpdateShader(const glm::mat4& world)
{
    m_scene.shaders[m_selectedShader]->SendUniform("world", world);
}

bool OpenGL::UpdateShader(const Mesh& mesh)
{
    const int index = mesh.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = *m_scene.shaders[index];
        if(index != m_selectedShader)
        {
            SetSelectedShader(index);
            SendLights();

            shader.SendUniform("viewProjection", m_camera.ViewProjection());
        }

        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(false, false);
        return true;
    }
    return false;
}

void OpenGL::SendLights()
{
    auto& shader = *m_scene.shaders[m_selectedShader];
    const auto& lights = m_scene.lights;

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        const int offset = i * 3;
        shader.SendUniform("lightPosition", lights[i]->Position(), offset);
        shader.SendUniform("lightDiffuse", lights[i]->Diffuse(), offset);
    }
}

void OpenGL::SendTexture(std::string sampler, int ID)
{
    if (ID != NO_INDEX)
    {
        auto& shader = *m_scene.shaders[m_selectedShader];
        shader.SendTexture(sampler, m_scene.textures[ID]->GetID());
    }
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

void OpenGL::EnableSelectedShader()
{
    m_scene.shaders[m_selectedShader]->EnableShader();
}

void OpenGL::SetSelectedShader(int index)
{
    m_selectedShader = index;
    m_scene.shaders[m_selectedShader]->SetActive();
}