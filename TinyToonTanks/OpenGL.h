////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - OpenGL.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <memory>

struct GLFWwindow;
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
    * @param camera The viewable camera
    */
    OpenGL(const Camera& camera);

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
    * Ends the rendering pipeline
    */
    void EndRender();

    /**
    * Toggles whether wireframe is active
    */
    void ToggleWireframe();

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
    * Sets whether alpha blending is enabled or not
    * @param enable whether blending is enabled
    * @param whether to multiply the blend colours
    */
    void EnableAlphaBlending(bool enable, bool multiply);

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

    GLFWwindow* m_window = nullptr;  ///< Handle to the application window
    const Camera& m_camera;          ///< The viewable camera
    bool m_isBackfaceCull = true;    ///< Whether the culling rasterize state is active
    bool m_isWireframe = false;      ///< Whether to render the scene as wireframe
    bool m_isAlphaBlend = false;     ///< Whether alpha blending is currently active
    bool m_isBlendMultiply = false;  ///< Whether to multiply the blend colours
    bool m_isDepthWrite = true;      ///< Whether writing to the depth buffer is active
    int m_selectedShader = -1;       ///< Currently active shader for rendering

    std::unique_ptr<Quad> m_quad;                ///< Post processing quad
    std::unique_ptr<RenderTarget> m_backBuffer;  ///< Back buffer target
    std::unique_ptr<RenderTarget> m_sceneTarget; ///< Main scene target
};