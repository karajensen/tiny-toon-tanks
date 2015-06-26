////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"
#include "common.h"
#include "tweaker.h"

namespace
{
    const float FRUSTRUM_NEAR = 1.0f;
    const float FRUSTRUM_FAR = 1500.0f; // Minimum value for skybox
    const float FRUSTRUM_BOUNDS_NEAR = 0.0f;
    const float FRUSTRUM_BOUNDS_FAR = 1500.0f;
    const float FIELD_OF_VIEW = 60.0f;
    const float RATIO = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);
}

Camera::Camera() :
    m_initialPos(15.0f, 1.0f, 3.0f),
    m_position(m_initialPos),
    m_target(0.0f, 0.0f, 0.0f),
    m_rotationSpeed(3.0f),
    m_translateSpeed(20.0f),
    m_forwardSpeed(60.0f),
    m_pitch(0.0f),
    m_yaw(75.0f),
    m_roll(0.0f),
    m_requiresUpdate(true)
{
    m_projection = glm::perspective(FIELD_OF_VIEW, 
        RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);
}

void Camera::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddFltEntry("Rotation Speed", &m_rotationSpeed, 1.0f);
    tweaker.AddFltEntry("Translate Speed", &m_translateSpeed, 1.0f);
    tweaker.AddFltEntry("Forward Speed", &m_forwardSpeed, 1.0f);
    tweaker.AddFltEntry("Position X", &m_position.x);
    tweaker.AddFltEntry("Position Y", &m_position.y);
    tweaker.AddFltEntry("Position Z", &m_position.z);
    tweaker.AddFltEntry("Pitch", &m_pitch);
    tweaker.AddFltEntry("Yaw", &m_yaw);
    tweaker.AddFltEntry("Roll", &m_roll);
}

void Camera::Forward(float value)
{
    m_position += m_forward * m_forwardSpeed * value;
    m_requiresUpdate = true;
}

void Camera::Right(float value)
{
    m_position += m_right * m_translateSpeed * value;
    m_requiresUpdate = true;
}

void Camera::Up(float value)
{
    m_position += m_up * m_translateSpeed * value;
    m_requiresUpdate = true;
}

void Camera::Yaw(float angle)
{
    m_yaw += m_rotationSpeed * RadToDeg(angle);
    m_requiresUpdate = true;
}

void Camera::Pitch(float angle)
{
    m_pitch += m_rotationSpeed * RadToDeg(angle);
    m_requiresUpdate = true;
}

void Camera::Roll(float angle)
{
    m_roll += m_rotationSpeed * RadToDeg(angle);
    m_requiresUpdate = true;
}

void Camera::Reset()
{
    m_requiresUpdate = true;
    m_position = m_initialPos; 
    m_yaw = 0;
    m_roll = 0;
    m_pitch = 0;
}

void Camera::Update(float deltatime)
{
    if (m_requiresUpdate)
    {
        m_requiresUpdate = false;
        
        glm::mat4 translate;
        translate[3][0] = m_position.x;
        translate[3][1] = m_position.y;
        translate[3][2] = m_position.z;

        glm::mat4 rotation, rotateX, rotateY, rotateZ;
        rotateX = glm::rotate(rotateX, m_pitch, glm::vec3(1,0,0));
        rotateY = glm::rotate(rotateY, m_yaw, glm::vec3(0,1,0));
        rotateZ = glm::rotate(rotateZ, m_roll, glm::vec3(0,0,1));
        rotation = rotateZ * rotateY * rotateX;

        m_world = translate * rotation;
        m_view = glm::inverse(m_world);
        m_viewProjection = m_projection * m_view;

        m_up.x = m_world[1][0];
        m_up.y = m_world[1][1];
        m_up.z = m_world[1][2];

        m_right.x = m_world[0][0];
        m_right.y = m_world[0][1];
        m_right.z = m_world[0][2];

        m_forward.x = -m_world[2][0];
        m_forward.y = -m_world[2][1];
        m_forward.z = -m_world[2][2];
    }
}

void Camera::Rotate(const glm::vec2& direction, float value)
{
    if(direction.x != 0.0f)
    {
        Yaw(direction.x < 0.0f ? -value : value);
    }
    if(direction.y != 0.0f)
    {
        Pitch(direction.y < 0.0f ? -value : value);
    }
}

const glm::mat4& Camera::ViewProjection() const
{
    return m_viewProjection;
}

const glm::mat4& Camera::Projection() const
{
    return m_projection;
}

const glm::mat4& Camera::View() const
{
    return m_view;
}

const glm::mat4& Camera::World() const
{
    return m_world;
}

const glm::vec3& Camera::Position() const
{
    return m_position;
}

const glm::vec3& Camera::Up() const
{
    return m_up;
}