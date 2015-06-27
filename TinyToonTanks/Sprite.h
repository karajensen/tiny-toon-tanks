////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sprite.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <memory>
#include "glm/glm.hpp"

class Quad;

/**
* Quad that always faces the camera in screen space
*/
class Sprite
{
public:

    /**
    * Constructor
    * @param name The name of the quad
    */
    Sprite(const std::string& name);

    /**
    * Prerenders the sprite
    */
    void PreRender();

    /**
    * Renders the sprite
    * @param cameraPosition The position of the camera
    * @param cameraUp The up vector of the camera
    */
    void Render(const glm::vec3& cameraPosition,
                const glm::vec3& cameraUp);

    /**
    * Sets the position of the sprite using world space coordinates
    */
    void SetPosition(const glm::vec3& position);

    /**
    * Sets the position of the sprite using screen space coordinates
    */
    void SetPosition(const glm::vec2& position);

    /**
    * Sets the scaling of the sprite
    */
    void SetScale(const glm::vec2& scale);

    /**
    * Sets the depth of the sprite
    */
    void SetDepth(float depth);

    /**
    * @return the depth value of the sprite
    */
    float GetDepth() const;

private:

    /**
    * Prevent copying
    */
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    std::unique_ptr<Quad> m_quad;  ///< The quad to render to
    float m_depth = 0.0f;          ///< Ordering for sprites
};