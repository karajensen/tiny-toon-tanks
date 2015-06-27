////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - rendertarget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "rendertarget.h"
#include "Common.h"

RenderTarget::RenderTarget(const std::string& name) :
    m_isBackBuffer(true),
    m_name(name)
{
}

RenderTarget::RenderTarget(const std::string& name, 
                           bool multisampled) :

    m_multisampled(multisampled),
    m_name(name)
{
}

RenderTarget::~RenderTarget()
{
    Release();
}

void RenderTarget::Release()
{
    if(m_initialised && !m_isBackBuffer)
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
        glDeleteTextures(1, &m_texture);
        glDeleteRenderbuffers(1, &m_renderBuffer);
    }
    m_initialised = false;
}

GLuint RenderTarget::GetTexture() const
{ 
    return m_texture;
}

bool RenderTarget::Initialise()
{
    if(!m_isBackBuffer)
    {
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        if(HasCallFailed())
        {
            LogError(m_name + " Render target creation failed");
            return false;
        }

        const unsigned int textureType = m_multisampled ?
            GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        if (!CreateTexture(m_texture, textureType))
        {
            return false;
        }

        m_attachment = GetTextureAttachment(0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachment, textureType, m_texture, 0);

        if(HasCallFailed())
        {
            LogError(m_name + " Failed to create texture buffer");
            return false;
        }

        // Create the render buffer to hold depth information
        glGenRenderbuffers(1, &m_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);

        if (m_multisampled)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_COUNT, 
                GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
            GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

        if(HasCallFailed())
        {
            LogError(m_name + " Failed to create depth texture");
            return false;
        }

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LogError(m_name + " sampling does not match across attachments");
            return false;
        }
    }

    m_initialised = true;
    return true;
}

bool RenderTarget::CreateTexture(GLuint& id, unsigned int type)
{
    glGenTextures(1, &id);
    glBindTexture(type, id);

    if (m_multisampled)
    {
        glTexImage2DMultisample(type, MULTISAMPLING_COUNT, 
            GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, GL_TRUE);  
    }
    else
    {
        glTexImage2D(type, 0, GL_RGBA, WINDOW_WIDTH,
            WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if(HasCallFailed())
    {
        LogError(m_name + " Failed to create texture");
        return false;
    }

    return true;
}

bool RenderTarget::IsMultisampled() const
{
    return m_multisampled;
}

void RenderTarget::SetActive()
{
    assert(m_initialised);

    m_multisampled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE); 

    if(m_isBackBuffer)
    {
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffers(1, &m_attachment);
    }

    if(HasCallFailed())
    {
        LogError("Could not set " + m_name + " as active");
    }
}

unsigned int RenderTarget::GetTextureAttachment(int index) const
{
    switch (index)
    {
    case 0:
        return 0x8CE0; //GL_COLOR_ATTACHMENT0_EXT
    case 1:
        return 0x8CE1; //GL_COLOR_ATTACHMENT1_EXT
    case 2:
        return 0x8CE2; //GL_COLOR_ATTACHMENT2_EXT
    case 3:
        return 0x8CE3; //GL_COLOR_ATTACHMENT3_EXT
    case 4:
        return 0x8CE4; //GL_COLOR_ATTACHMENT4_EXT
    default:
        LogError("Unsupported index for render target texture");
        return 0;
    }
}