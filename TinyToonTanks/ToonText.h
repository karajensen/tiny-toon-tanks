////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ToonText.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Quad.h"

class Camera;

/**
* Toon shaded quad that appears in the world
*/
class ToonText : public Quad
{
public:

    /**
    * Constructor
    * @param name The name of the quad
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    ToonText(const std::string& name,
             const std::string& shaderName, 
             int shaderID);
    /**
    * Displays an instance of the toon text at the given position
    */
    void Show(const glm::vec3& position);

    /**
    * Ticks all instances of the toon text and faces them towards the camera
    */
    void Tick(const Camera& camera);
};