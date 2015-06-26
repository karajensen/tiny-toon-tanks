////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - rendertarget.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "glcommon.h"

/**
* Render target with optional attached depth buffer
*/
class RenderTarget
{
public:

    /**
    * Constructor for the back buffer
    * @param name Name of the render target
    */
    RenderTarget(const std::string& name);

    /**
    * Constructor for a render target
    * @param name Name of the render target
    * @param multisampled Whether this target has multisampling
    */
    RenderTarget(const std::string& name, bool multisampled);

    /**
    * Destructor
    */
    ~RenderTarget();

    /**
    * Releases the render target
    */
    void Release();

    /**
    * Initialises the render target
    * @return whether initialisation succeeded or not
    */
    bool Initialise();

    /**
    * Sets the render target as activated and clears it
    */
    void SetActive();

    /**
    * @return the ID of the target texture
    */
    GLuint GetTexture() const;
    
    /**
    * @return if this target is multisampled
    */
    bool IsMultisampled() const;

private:

    /**
    * Prevent copying
    */
    RenderTarget(const RenderTarget&) = delete;
    RenderTarget& operator=(const RenderTarget&) = delete;

    /**
    * @param index The index of the texture
    * @return the OpenGL Attachement ID for the texture
    */
    unsigned int GetTextureAttachment(int index) const;

    /**
    * Creates the texture
    * @param id The ID to fill in for the new texture
    * @param type The type of texture to create
    * @param highQuality Whether to make it a high quality texture
    * @return whether create was successful
    */
    bool CreateTexture(GLuint& id, unsigned int type);

    bool m_initialised = false;         ///< Whether the buffer is initialised or not
    const bool m_isBackBuffer = false;  ///< Whether this render target is the back buffer
    const bool m_multisampled = false;  ///< Whether this target has multisampling
    const std::string m_name;           ///< Name of the render target
    GLuint m_texture;                   ///< Unique ID of the main attached texture
    GLenum m_attachment;                ///< Attachment slot taken up
    GLuint m_renderBuffer = 0;          ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer = 0;           ///< Unique ID of the frame buffer
};