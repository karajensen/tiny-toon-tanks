////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"

#include <string>

class Tweaker;

/**
* Single light to provide illumination in the scene
*/
class Light
{
public:

    /**
    * Constructor
    * @param name The name of the light
    */
    Light(const std::string& name); 

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helpe r for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * @return The name of the light
    */
    const std::string& Name() const;

    /**
    * @return Colour of the light
    */
    const glm::vec3& Diffuse() const;

    /**
    * @return World coordinates of the light
    */
    const glm::vec3& Position() const;

    /**
    * Sets the Colour of the light
    */
    void Diffuse(const glm::vec3& value);

    /**
    * Sets the World coordinates of the light
    */
    void Position(const glm::vec3& value);

    /**
    * Sets the World coordinates of the light
    */
    void PositionX(float x);

    /**
    * Sets the World coordinates of the light
    */
    void PositionZ(float z);

private:

    /**
    * Prevent copying
    */
    Light(const Light&) = delete;
    Light& operator=(const Light&) = delete;

private:
                                 
    const std::string m_name;     ///< Name of the light
    glm::vec3 m_diffuse;          ///< Colour of the light
    glm::vec3 m_position;         ///< World coordinates of the light
};                                  