////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - quad.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "mesh.h"

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
    Quad(const std::string& name, int shader = -1);

private:

    /**
    * Prevent copying
    */
    Quad(const Quad&) = delete;
    Quad& operator=(const Quad&) = delete;
};