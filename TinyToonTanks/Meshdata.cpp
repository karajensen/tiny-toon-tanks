////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Meshdata.h"
#include "Tweaker.h"
#include "Glcommon.h"
#include "Conversions.h"
#include <algorithm>

MeshData::MeshData(const std::string& name, 
                   const std::string& shaderName,
                   int shaderID) :
    m_name(name),
    m_shaderName(shaderName),
    m_shaderIndex(shaderID),
    m_scale(1.0f, 1.0f, 1.0f)
{ 
}

MeshData::~MeshData()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

void MeshData::AddToTweaker(Tweaker& tweaker)
{
    tweaker.AddStrEntry("Name", m_name);
    tweaker.AddStrEntry("Shader", m_shaderName);
    tweaker.AddEntry("Backface Cull", &m_backfacecull, TW_TYPE_BOOLCPP);
    tweaker.AddFltEntry("Radius", &m_radius, 0.1f, 0.1f, FLT_MAX);
}

bool MeshData::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(1, &m_vboID);
    glGenBuffers(1, &m_iboID);
    m_initialised = true;
    
    glBindVertexArray(m_vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), 
        &m_indices[0], GL_STATIC_DRAW);

    if(HasCallFailed())
    {
        LogError("Failed to set " + m_name + " buffers");
        return false;
    }

    GenerateRadius();

    return true;
}

void MeshData::GenerateRadius()
{
    m_radius = 0.0f;

    // Assumes position is always first in a vertex
    for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
    {
        const glm::vec3 position(m_vertices[vertex], 
            m_vertices[vertex + 1], m_vertices[vertex + 2]);

        m_radius = std::max(m_radius, glm::length(position));
    }
}

void MeshData::PreRender() const
{
    if (ShouldRender())
    {
        assert(m_initialised);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    }
}

void MeshData::Render() const
{
    if (ShouldRender())
    {
        assert(m_initialised);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }
}

const std::string& MeshData::Name() const
{
    return m_name;
}

int MeshData::ShaderID() const
{
    return m_shaderIndex;
}

const std::vector<float>& MeshData::Vertices() const
{
    return m_vertices;
}

const std::vector<unsigned long>& MeshData::Indices() const
{
    return m_indices;
}

void MeshData::SetTexture(int ID)
{
    if (ID == -1)
    {
        LogError("Texture ID invalid");
    }

    m_textureID = ID;
}

bool MeshData::BackfaceCull() const
{
    return m_backfacecull;
}

void MeshData::BackfaceCull(bool value)
{
    m_backfacecull = value;
}

bool MeshData::ShouldRender() const
{
    return m_render;
}

void MeshData::SetShouldRender(bool render)
{
    m_render = render;
}

const glm::vec3& MeshData::Position() const
{
    return m_position;
}

const glm::vec3& MeshData::Scale() const
{
    return m_scale;
}

void MeshData::SetWorld(const glm::mat4& world)
{
    m_requiresUpdate = false;
    m_world = world;
}

void MeshData::Tick(const glm::vec3& cameraPosition)
{
    if (m_requiresUpdate)
    {
        m_requiresUpdate = false;

        if (m_rotation.x == 0 &&
            m_rotation.y == 0 &&
            m_rotation.z == 0)
        {
            m_world[0][0] = m_scale.x;
            m_world[0][1] = 0.0f;
            m_world[0][2] = 0.0f;
            m_world[1][0] = 0.0f;
            m_world[1][1] = m_scale.y;
            m_world[1][2] = 0.0f;
            m_world[2][0] = 0.0f;
            m_world[2][1] = 0.0f;
            m_world[2][2] = m_scale.z;
            m_world[3][0] = m_position.x;
            m_world[3][1] = m_position.y;
            m_world[3][2] = m_position.z;
        }
        else
        {
            glm::mat4 scale;
            scale[0][0] = m_scale.x;
            scale[1][1] = m_scale.y;
            scale[2][2] = m_scale.z;

            glm::mat4 translate;
            translate[3][0] = m_position.x;
            translate[3][1] = m_position.y;
            translate[3][2] = m_position.z;

            glm::mat4 rotateX, rotateY, rotateZ;
            rotateX = glm::rotate(rotateX, m_rotation.x, glm::vec3(1, 0, 0));
            rotateY = glm::rotate(rotateY, m_rotation.y, glm::vec3(0, 1, 0));
            rotateZ = glm::rotate(rotateZ, m_rotation.z, glm::vec3(0, 0, 1));

            m_world = translate * (rotateZ * rotateX * rotateY) * scale;
        }
    }
}

std::unique_ptr<float[]> MeshData::CreateFloatBuffer() const
{
    const int size = static_cast<int>(m_vertices.size());
    const int amount = size / m_vertexComponentCount;
    std::unique_ptr<float[]> vertices(new float[amount]);

    for (int i = 0, j = 0; i < size; i += m_vertexComponentCount, j += 3)
    {
        vertices[j] = m_vertices[i];    // x
        vertices[j+1] = m_vertices[i];  // y
        vertices[j+2] = m_vertices[i];  // z
    }

    return vertices;
}