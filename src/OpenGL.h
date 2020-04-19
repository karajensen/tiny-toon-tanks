////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - OpenGL.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include <string>
#include "glm/glm.hpp"

struct GLFWwindow;
struct SceneData;
class Mesh;
class Quad;
class Camera;
class RenderTarget;

/**
* Engine for initialising and managing OpenGL
*/
class OpenGL
{
public:

    /**
    * Constructor
    * @param scene The data to render
    * @param camera The viewable camera
    */
    OpenGL(const SceneData& scene,
           const Camera& camera);

    /**
    * Destructor
    */
    ~OpenGL();

    /**
    * Initialise OpenGL
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

    /**
    * @return whether OpenGL is currently running
    */
    bool IsRunning() const;

    /**
    * Renders the scene
    */
    void RenderScene();

    /**
    * Applies the post processing shader
    */
    void RenderPostProcessing();

    /**
    * Ends the rendering pipeline
    */
    void EndRender();

    /**
    * @return the application window
    */
    GLFWwindow& GetWindow() const;

private: 

    /**
    * Prevent copying
    */
    OpenGL(const OpenGL&) = delete;
    OpenGL& operator=(const OpenGL&) = delete;

    /**
    * Releases OpenGL
    */
    void Release();

    /**
    * Renders the meshes
    */
    void RenderMeshes();

    /**
    * Updates and switches to main shader the mesh requires
    */
    bool UpdateShader(const Mesh& mesh);

    /**
    * Updates and switches to the shadow shader the mesh requres
    */
    bool UpdateShadowShader();

    /**
    * Updates the shader for a mesh per instance
    */
    void UpdateShader(const glm::mat4& world, int texture);

    /**
    * Updates the shader for a mesh per instance
    */
    void UpdateShader(const glm::mat4& world);

    /**
    * Updates the shader for a mesh per instance
    */
    void UpdateShadowShader(const glm::mat4& world);

    /**
    * Sets whether alpha blending is enabled or not
    */
    void EnableAlphaBlending(bool enable);

    /**
    * Sends light information to the selected shader
    */
    void SendLights();

    /**
    * Sends the texture to the selected shader
    */
    void SendTexture(std::string sampler, int ID);

    /**
    * Sets whether values are written to the depth buffer or not
    * @note if set to false the depth buffer of the currently 
    *       selected render target till not clear
    */
    void EnableDepthWrite(bool enable);

    /**
    * Sets whether opengl should cull backfaces or not
    * @param enable whether to cull or not
    */
    void EnableBackfaceCull(bool enable);

    /**
    * Enables the selected shader for rendering
    */
    void EnableSelectedShader();

    /**
    * Sets the shader at the given index as selected
    */
    void SetSelectedShader(int index);

    GLFWwindow* m_window = nullptr;  ///< Handle to the application window
    const Camera& m_camera;          ///< The viewable camera
    const SceneData& m_scene;        ///< The data to render
    bool m_isBackfaceCull = true;    ///< Whether the culling rasterize state is active
    bool m_isAlphaBlend = false;     ///< Whether alpha blending is currently active
    bool m_isDepthWrite = true;      ///< Whether writing to the depth buffer is active
    int m_selectedShader = -1;       ///< Currently active shader for rendering

    std::unique_ptr<Quad> m_quad;                 ///< Post processing quad
    std::unique_ptr<RenderTarget> m_backBuffer;   ///< Back buffer target
    std::unique_ptr<RenderTarget> m_sceneTarget;  ///< Main scene target, includes the normal texture
};