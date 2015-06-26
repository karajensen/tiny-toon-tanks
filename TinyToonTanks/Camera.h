////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Camera.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include <memory>

class Tweaker;

/**
* Camera class to view the world with
*/
class Camera
{
public:

    /**
    * Constructor
    */
    Camera();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Updates the view matrix
    * @param deltatime Time passed between frames
    */
    void Update(float deltatime);

    /**
    * Resets the camera to the initial state
    */
    void Reset();

    /**
    * Rotate the camera around the y axis
    * @param angle The angle to rotate in radians
    */
    void Yaw(float angle);

    /**
    * Rotate the camera around the x axis
    * @param angle The angle to rotate in radians
    */
    void Pitch(float angle);

    /**
    * Rotate the camera around the z axis
    * @param angle The angle to rotate in radians
    */
    void Roll(float angle);

    /**
    * Translates the camera along the forward vector
    * @param value The amount to move the camera
    */
    void Forward(float value);

    /**
    * Translates the camera along the right vector
    * @param value The amount to move the camera
    */
    void Right(float value);

    /**
    * Translates the camera along the up vector
    * @param value The amount to move the camera
    */
    void Up(float value);

    /**
    * Rotates the camera according to the mouse direction
    * @param direction The normalized direction of the mouse
    * @param value The amount to rotate the camera
    */
    void Rotate(const glm::vec2& direction, float value);

    /**
    * @return the view projection matrix
    */
    const glm::mat4& ViewProjection() const;

    /**
    * @return the projection matrix
    */
    const glm::mat4& Projection() const;

    /**
    * @return the view matrix
    */
    const glm::mat4& View() const;

    /**
    * @return the world matrix
    */
    const glm::mat4& World() const;

    /**
    * @return the camera position
    */
    const glm::vec3& Position() const;

    /**
    * @return the camera up vector
    */
    const glm::vec3& Up() const;

private:

    /**
    * Prevent copying
    */
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    glm::mat4 m_projection;                 ///< Projection Matrix for the camera
    glm::mat4 m_view;                       ///< View Matrix for the camera
    glm::mat4 m_viewProjection;             ///< Combined view projection matrix
    glm::mat4 m_world;                      ///< World Matrix for the camera
    glm::vec3 m_initialPos;                 ///< Camera initial position in world space
    glm::vec3 m_position;                   ///< Camera position in world space
    glm::vec3 m_up;                         ///< Camera up vector
    glm::vec3 m_forward;                    ///< Camera forward vector
    glm::vec3 m_right;                      ///< Camera right vector
    glm::vec3 m_target;                     ///< Camera Look at target
    bool m_requiresUpdate = true;           ///< Whether the camera requires updating or not
    float m_yaw = 0.0f;                     ///< Degrees amount of yaw
    float m_pitch = 0.0f;                   ///< Degrees amount of pitch
    float m_roll = 0.0f;                    ///< Degrees amount of roll
    float m_rotationSpeed = 0.0f;           ///< Speed to rotate the camera by
    float m_translateSpeed = 0.0f;          ///< Speed to translate the camera by
    float m_forwardSpeed = 0.0f;            ///< Speed to move foward with
};