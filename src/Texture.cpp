////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Texture.h"
#include "Glcommon.h"
#include "Tweaker.h"

#include "soil/SOIL.h"

Texture::Texture(const std::string& name, 
                 const std::string& path, 
                 Filter filter)
    : m_name(name)
    , m_path(path)
    , m_filter(filter)
{
}

Texture::~Texture()
{
    if(m_initialised)
    {
        glDeleteTextures(1, &m_id);
        m_initialised = false;
    }
}

const std::string& Texture::Name() const
{
    return m_name;
}

const std::string& Texture::Path() const
{
    return m_path;
}

bool Texture::Initialise()
{
    glGenTextures(1, &m_id);
    m_initialised = true;
    glBindTexture(GL_TEXTURE_2D, m_id);

    return LoadTexture(GL_TEXTURE_2D, m_path) && 
        CreateMipMaps() && !HasCallFailed();
}

bool Texture::SetFiltering()
{
    const auto type = GL_TEXTURE_2D;
    const auto magFilter = m_filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR;
    const auto minFilter = m_filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR;

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);

    if (m_filter == Texture::ANISOTROPIC)
    {
        const float MAX_ANISOTROPY = 16.0f;
        const auto GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE;
        glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, MAX_ANISOTROPY);
    }

    if(HasCallFailed())
    {
        LogError("Failed to set filtering for " + m_name);
        return false;
    }
    return true;
}

bool Texture::CreateMipMaps()
{
    if (m_filter != Texture::NEAREST)
    {
        glGenerateMipmap(GL_TEXTURE_2D);

        if(HasCallFailed())
        {
            LogError("Mipmap creation failed for " + m_name);
            return false;
        }
    }
    return true;
}

bool Texture::LoadTexture(int type, const std::string& path)
{
    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if(HasCallFailed())
    {
        LogError("Failed to load " + path + " texture");
        return false;
    }

    return SetFiltering();
}

unsigned int Texture::GetID() const
{
    return m_id;
}