////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "glcommon.h"

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of filtering for this texture
    */
    enum Filter
    {
        NEAREST,
        LINEAR,
        ANISOTROPIC
    };

    /**
    * Constructor for a texture
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param filter The type of filtering for this texture
    */
    Texture(const std::string& name, 
            const std::string& path,
            Filter filter);

    /**
    * Destructor
    */
    virtual ~Texture();

    /**
    * Initialises the texture
    * @return whether initialisation was successful
    */
    virtual bool Initialise();

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the path of the texture
    */
    const std::string& Path() const;

    /**
    * @return the unique ID for the texture
    */
    virtual GLuint GetID() const;

private:

    /**
    * Prevent copying
    */
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    /**
    * Creates the mipmaps for the texture
    * @return whether creation was successful
    */
    bool CreateMipMaps();

    /**
    * Sets the filtering for the texture
    * @return whether setting was successful
    */
    bool SetFiltering();

    /**
    * Loads a texture from file
    * @param type The type of opengl texture to load
    * @param path The path to the texture
    * @return whether loading was successful
    */
    bool LoadTexture(GLenum type, const std::string& path);

    Filter m_filter;             ///< The type of filtering for this texture
    bool m_initialised = false;  ///< Whether this texture is initialised
    GLuint m_id = 0;             ///< Unique id for the texture
    std::string m_name;          ///< Name of the texture
    std::string m_path;          ///< Path to the texture
};