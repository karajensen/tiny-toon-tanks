////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Quad.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Mesh.h"

/**
* Four-vertex quad
*/
class Quad : public Mesh
{
public:

    /**
    * Constructor
    * @param name The name of the quad
    * @param shader The ID of the shader to use
    */
    Quad(const std::string& name);

    /**
    * Constructor
    * @param name The name of the quad
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    Quad(const std::string& name,
         const std::string& shaderName, 
         int shaderID);

private:

    /**
    * Prevent copying
    */
    Quad(const Quad&) = delete;
    Quad& operator=(const Quad&) = delete;
};