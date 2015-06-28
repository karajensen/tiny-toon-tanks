////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Sprite.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Sprite.h"
#include "Quad.h"

Sprite::Sprite(const std::string& name) :
    m_quad(std::make_unique<Quad>(name))
{
}

void Sprite::PreRender()
{
    m_quad->PreRender();
}

void Sprite::Render(const glm::vec3& cameraPosition,
                    const glm::vec3& cameraUp)
{
    m_quad->Render();
}

void Sprite::SetPosition(const glm::vec3& position)
{

}

void Sprite::SetPosition(const glm::vec2& position)
{

}

void Sprite::SetScale(const glm::vec2& scale)
{

}

void Sprite::SetDepth(float depth)
{
    m_depth = depth;
}

float Sprite::GetDepth() const
{
    return m_depth;
}