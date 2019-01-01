////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ToonText.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "ToonText.h"
#include "GlmHelper.h"
#include "Camera.h"
#include <algorithm>

ToonText::ToonText(const std::string& name, int shaderID)
    : Quad(name, shaderID)
{
}

void ToonText::Show(const glm::vec3& position)
{
    for (int i = 0; i < Instances(); ++i)
    {
        if (!IsVisible(i))
        {
            Scale(glm::vec3(0.0f, 0.0f, 0.0f), i);
            Position(position.x, position.y, position.z, i);
            Visible(true, i);
            return;
        }
    }
}

void ToonText::Tick(const Camera& camera, float deltatime)
{
    const auto& cameraPosition = camera.Position();
    const auto& cameraUp = camera.Up();

    const float maxTimeScaling = 0.2f;
    const float minScale = 2.0f;
    const float maxScaleIncrease = 4.0f;
    const float maxScale = minScale + maxScaleIncrease;

    for (int i = 0; i < Instances(); ++i)
    {
        if (IsVisible(i))
        {
            auto scale = Scale(i);
            scale.y = scale.y + (scale.z == 0.0f ? deltatime : -deltatime);
            scale.x = minScale + std::min((scale.y / maxTimeScaling) * maxScaleIncrease, maxScaleIncrease);

            if (scale.z == 0.0f && scale.x >= maxScale)
            {
                scale.y = maxTimeScaling;
                scale.z = 1.0f;
            }
            else if (scale.z != 0.0f && scale.x <= 0.0f)
            {
                Visible(false, i);
                continue;
            }

            const auto& position = Position(i);

            glm::vec3 right, up, forward;
            forward.x = cameraPosition.x - position.x;
            forward.y = cameraPosition.y - position.y;
            forward.z = cameraPosition.z - position.z;

            forward = glm::normalize(forward);
            right = glm::cross(forward, cameraUp);
            up = glm::cross(forward, right);

            glm::mat4 scaleMatrix;
            scaleMatrix[0][0] = -scale.x;
            scaleMatrix[1][1] = scale.x;
            scaleMatrix[2][2] = -scale.x;

            glm::mat4 rotateMatrix;
            rotateMatrix[0][0] = right.x;
            rotateMatrix[0][1] = right.y;
            rotateMatrix[0][2] = right.z;
            rotateMatrix[1][0] = up.x;
            rotateMatrix[1][1] = up.y;
            rotateMatrix[1][2] = up.z;
            rotateMatrix[2][0] = forward.x;
            rotateMatrix[2][1] = forward.y;
            rotateMatrix[2][2] = forward.z;

            glm::mat4 translateMatrix;
            translateMatrix[3][0] = position.x;
            translateMatrix[3][1] = position.y;
            translateMatrix[3][2] = position.z;

            Scale(scale, i);
            SetWorld(translateMatrix * rotateMatrix * scaleMatrix, i);
        }
    }
}