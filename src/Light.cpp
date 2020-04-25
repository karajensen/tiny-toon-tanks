////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "light.h"
#include "tweaker.h"

Light::Light(const std::string& name)
    : m_name(name)
{
}

void Light::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", m_name);
    tweaker.AddFltEntry("Position X", &m_position.x, 0.01f);
    tweaker.AddFltEntry("Position Y", &m_position.y, 0.01f);
    tweaker.AddFltEntry("Position Z", &m_position.z, 0.01f);
    tweaker.AddEntry("Diffuse", &m_diffuse.x, TW_TYPE_COLOR3F);
}

const std::string& Light::Name() const
{
    return m_name;
}

const glm::vec3& Light::Diffuse() const
{
    return m_diffuse;
}

const glm::vec3& Light::Position() const
{
    return m_position;
}


void Light::Diffuse(const glm::vec3& value)
{
    m_diffuse = value;
}

void Light::Position(const glm::vec3& value)
{
    m_position = value;
}

void Light::PositionX(float x)
{
    m_position.x = x;
}

void Light::PositionZ(float z)
{
    m_position.z = z;
}
