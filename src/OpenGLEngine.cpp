////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - OpenGLEngine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "OpenGLEngine.h"
#include "Camera.h"
#include "Quad.h"
#include "SceneData.h"
#include "Rendertarget.h"
#include "GlmHelper.h"
#include "Utils.h"

namespace
{
    const int NO_INDEX = -1;
}

OpenGLEngine::OpenGLEngine(const SceneData& scene, const Camera& camera)
    : m_camera(camera)
    , m_scene(scene)
    , m_quad(std::make_unique<Quad>("PostQuad"))
{
}

OpenGLEngine::~OpenGLEngine()
{
    Release();
}

void OpenGLEngine::Release()
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

bool OpenGLEngine::IsRunning() const
{
    return !glfwWindowShouldClose(m_window) && 
          glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

bool OpenGLEngine::Initialise()
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

    glClearColor(0.24f, 0.24f, 0.24f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glDisablei(GL_BLEND, 0);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

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

GLFWwindow& OpenGLEngine::GetWindow() const
{
    assert(m_window);
    return *m_window;
}

void OpenGLEngine::RenderScene()
{
    m_sceneTarget->SetActive();
    RenderMeshes();

    m_backBuffer->SetActive();
    RenderPostProcessing();
}

void OpenGLEngine::RenderPostProcessing()
{
    EnableBackfaceCull(false);
    EnableAlphaBlending(false);
    EnableDepthWrite(true);

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

void OpenGLEngine::RenderMeshes()
{
    for (const auto& mesh : m_scene.meshes)
    {
        if (mesh->AnyInstanceVisible() && UpdateShader(*mesh))
        {
            mesh->PreRender();
            EnableSelectedShader();
            mesh->RenderTextured([this](const glm::mat4& world, int texture)
            {
                UpdateShader(world, texture);
            });
        }
    }

    for (const auto& mesh : m_scene.meshes)
    {
        if (mesh->AnyInstanceVisible() && mesh->RenderShadows() && UpdateShadowShader())
        {
            mesh->PreRender();
            EnableSelectedShader();
            mesh->Render([this](const glm::mat4& world)
            {
                UpdateShadowShader(world);
            });
        }
    }

    for (const auto& effect : m_scene.effects)
    {
        if (effect->AnyInstanceVisible() && UpdateShader(*effect))
        {
            effect->PreRender();
            EnableSelectedShader();
            effect->RenderTextured([this](const glm::mat4& world, int texture)
            {
                UpdateShader(world, texture);
            });
        }
    }
}

void OpenGLEngine::EndRender()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void OpenGLEngine::UpdateShader(const glm::mat4& world, int texture)
{
    UpdateShader(world);
    if (texture >= 0)
    {
        SendTexture("DiffuseSampler", texture);
    }
}

void OpenGLEngine::UpdateShadowShader(const glm::mat4& world)
{
    UpdateShader(world);

    const auto position = glm::matrix_get_position(world);
    const auto worldTranspose = glm::transpose(glm::mat3(world));

    const float shadowOffset = 0.8f;
    auto pointOnPlane = m_scene.meshes[MeshID::GROUND]->Position();
    pointOnPlane.y += shadowOffset;

    // change point on plane and normal to mesh's local frame
    const auto plane = worldTranspose * (pointOnPlane - position);
    const auto normal = worldTranspose * glm::vec3(0.0f, 1.0f, 0.0f);
    
    auto& shader = *m_scene.shaders[ShaderID::SHADOW];
    shader.SendUniform("planePosition", plane);
    shader.SendUniform("planeNormal", normal);
}

void OpenGLEngine::UpdateShader(const glm::mat4& world)
{
    m_scene.shaders[m_selectedShader]->SendUniform("world", world);
}

bool OpenGLEngine::UpdateShader(const Mesh& mesh)
{
    const int index = mesh.ShaderID();
    if (index != NO_INDEX)
    {
        auto& shader = *m_scene.shaders[index];
        if(index != m_selectedShader)
        {
            SetSelectedShader(index);

            if (mesh.RenderWithLights())
            {
                SendLights();
            }

            shader.SendUniform("viewProjection", m_camera.ViewProjection());
        }

        EnableBackfaceCull(mesh.BackfaceCull());
        EnableAlphaBlending(mesh.AlphaBlending());
        EnableDepthWrite(mesh.DepthWrite());
        return true;
    }
    return false;
}

bool OpenGLEngine::UpdateShadowShader()
{
    if (m_selectedShader != ShaderID::SHADOW)
    {
        auto& shader = *m_scene.shaders[ShaderID::SHADOW];

        SetSelectedShader(ShaderID::SHADOW);

        shader.SendUniform("viewProjection", m_camera.ViewProjection());

        EnableBackfaceCull(false);
        EnableAlphaBlending(false);
        EnableDepthWrite(true);
    }
    return true;
}

void OpenGLEngine::SendLights()
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

void OpenGLEngine::SendTexture(std::string sampler, int ID)
{
    if (ID != NO_INDEX)
    {
        auto& shader = *m_scene.shaders[m_selectedShader];
        shader.SendTexture(sampler, m_scene.textures[ID]->GetID());
    }
}

void OpenGLEngine::EnableAlphaBlending(bool enable)
{
    if (enable != m_isAlphaBlend)
    {
        m_isAlphaBlend = enable;
        enable ? glEnablei(GL_BLEND, 0) : glDisablei(GL_BLEND, 0);
        enable ? glEnablei(GL_BLEND, 1) : glDisablei(GL_BLEND, 1);
    }
}

void OpenGLEngine::EnableBackfaceCull(bool enable)
{
    if(enable != m_isBackfaceCull)
    {
        m_isBackfaceCull = enable;
        enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
}

void OpenGLEngine::EnableDepthWrite(bool enable)
{
    if (enable != m_isDepthWrite)
    {
        m_isDepthWrite = enable;
        enable ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
    }
}

void OpenGLEngine::EnableSelectedShader()
{
    m_scene.shaders[m_selectedShader]->EnableShader();
}

void OpenGLEngine::SetSelectedShader(int index)
{
    m_selectedShader = index;
    m_scene.shaders[m_selectedShader]->SetActive();
}