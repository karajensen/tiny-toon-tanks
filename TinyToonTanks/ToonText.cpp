////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ToonText.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "ToonText.h"
#include "GlmHelper.h"
#include "Camera.h"

ToonText::ToonText(const std::string& name,
                   const std::string& shaderName,
                   int shaderID)
    : Quad(name, shaderName, shaderID)
{
}

void ToonText::Show(const glm::vec3& position)
{
    for (int i = 0; i < Instances(); ++i)
    {
        if (!IsVisible(i))
        {
            glm::mat4 world;
            glm::matrix_set_position(world, position);
            SetWorld(world, i);
            Visible(true, i);
            return;
        }
    }
}

void ToonText::Tick(const Camera& camera)
{
    const auto& cameraPosition = camera.Position();
    const auto& cameraUp = camera.Up();
    const float toonTextScale = 5.0f;

    for (int i = 0; i < Instances(); ++i)
    {
        if (IsVisible(i))
        {
            const auto& position = Position(i);

            glm::vec3 right, up, forward;
            forward.x = cameraPosition.x - position.x;
            forward.y = cameraPosition.y - position.y;
            forward.z = cameraPosition.z - position.z;

            forward = glm::normalize(forward);
            right = glm::cross(forward, cameraUp);
            up = glm::cross(forward, right);

            glm::mat4 scale;
            scale[0][0] = -toonTextScale;
            scale[1][1] = toonTextScale;
            scale[2][2] = -toonTextScale;

            glm::mat4 rotate;
            rotate[0][0] = right.x;
            rotate[0][1] = right.y;
            rotate[0][2] = right.z;
            rotate[1][0] = up.x;
            rotate[1][1] = up.y;
            rotate[1][2] = up.z;
            rotate[2][0] = forward.x;
            rotate[2][1] = forward.y;
            rotate[2][2] = forward.z;

            glm::mat4 translate;
            translate[3][0] = position.x;
            translate[3][1] = position.y;
            translate[3][2] = position.z;

            SetWorld(translate * rotate * scale, i);
        }
    }
}