////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - rendertarget.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Glcommon.h"

#include <vector>
#include <string>

/**
* Render target with optional attached depth buffer
*/
class RenderTarget
{
public:

    /**
    * Constructor for the back buffer
    * @param name Name of the render target
    * @param isBackBuffer Whether this render target is the back buffer
    */
    RenderTarget(const std::string& name);

    /**
    * Constructor for a render target
    * @param name Name of the render target
    * @param textures The number of textures attached to this target
    * @param multisampled Whether this target has multisampling
    */
    RenderTarget(const std::string& name, 
                 int textures, 
                 bool multisampled);

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
    GLuint GetTexture(int index = 0) const;
    
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
    * @return whether create was successful
    */
    bool CreateTexture(GLuint& id, unsigned int type);

private:

    bool m_initialised = false;         ///< Whether the buffer is initialised or not
    const int m_count = 0;              ///< Number of textures attached to this buffer
    const bool m_isBackBuffer = false;  ///< Whether this render target is the back buffer
    const bool m_multisampled = false;  ///< Whether this target has multisampling
    const std::string m_name;           ///< Name of the render target
    std::vector<GLuint> m_textures;     ///< Unique IDs of the main attached textures
    std::vector<GLenum> m_attachments;  ///< Container of attachment slots taken up
    GLuint m_renderBuffer = 0;          ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer = 0;           ///< Unique ID of the frame buffer
};