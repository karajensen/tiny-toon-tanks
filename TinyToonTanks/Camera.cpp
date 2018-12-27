////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "Common.h"
#include "Tweaker.h"
#include "Input.h"
#include "GLMHelper.h"
#include "glm/ext.hpp"

namespace
{
    const float FRUSTRUM_NEAR = 1.0f;
    const float FRUSTRUM_FAR = 1000.0f;
    const float FIELD_OF_VIEW = 45.0f;
    const float RATIO = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);
}

Camera::Camera() :
    m_initialPos(0.0f, 20.0f, 0.0f),
    m_position(m_initialPos),
    m_target(0.0f, 0.0f, 0.0f),
    m_rotationSpeed(5.0f),
    m_translateSpeed(5.0f),
    m_forwardSpeed(5.0f),
    m_pitch(0.0f),
    m_yaw(0.0f),
    m_roll(0.0f),
    m_requiresUpdate(true),
    m_useFlyCamera(false)
{
    m_projection = glm::perspective(FIELD_OF_VIEW, RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);
}

void Camera::AddToTweaker(Tweaker& tweaker)
{
    tweaker.SetGroup("Camera");
    tweaker.AddEntry("Fly Camera", &m_useFlyCamera, TW_TYPE_BOOLCPP);
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

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    m_requiresUpdate = true;
}

bool Camera::IsFlyCamera() const
{
    return m_useFlyCamera;
}

void Camera::SetTarget(const glm::vec3& position)
{
    m_target = position;
}

void Camera::Update(const Input& input, float deltatime)
{
    if (m_useFlyCamera && input.IsRightMouseDown())
    {
        const auto& mouseDirection = input.GetMouseDirection();
        if(mouseDirection.x != 0.0f)
        {
            Yaw(mouseDirection.x < 0.0f ? -deltatime : deltatime);
            m_requiresUpdate = true;
        }
        if(mouseDirection.y != 0.0f)
        {
            Pitch(mouseDirection.y < 0.0f ? -deltatime : deltatime);
            m_requiresUpdate = true;
        }
    }

    if (m_requiresUpdate)
    {
        m_requiresUpdate = false;
        
        glm::mat4 rotation, rotateX, rotateY, rotateZ;
        rotateX = glm::rotate(rotateX, m_pitch, glm::vec3(1,0,0));
        rotateY = glm::rotate(rotateY, m_yaw, glm::vec3(0,1,0));
        rotateZ = glm::rotate(rotateZ, m_roll, glm::vec3(0,0,1));
        rotation = rotateZ * rotateY * rotateX;

        if (m_useFlyCamera)
        {
            m_world = rotation;
            glm::matrix_set_position(m_world, m_position);
            m_view = glm::inverse(m_world);
        }
        else
        {
            m_view = glm::lookAt(m_position, m_target, glm::matrix_get_up(rotation));
            m_world = glm::inverse(m_view);
        }

        m_viewProjection = m_projection * m_view;
        m_forward = glm::matrix_get_forward(m_world);
        m_up = glm::matrix_get_up(m_world);
        m_right = glm::matrix_get_right(m_world);
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