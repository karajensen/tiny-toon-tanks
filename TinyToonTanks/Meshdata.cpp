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
    m_shaderIndex(shaderID)
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

bool MeshData::Initialise(int instances)
{
    m_instances.resize(instances);

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
    assert(m_initialised);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void MeshData::Render(RenderInstance renderInstance) const
{
    for (const Instance& instance : m_instances)
    {
        if (instance.render)
        {
            renderInstance(instance.world);
            Render();
        }
    }
}

void MeshData::Render() const
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
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

int MeshData::GetTexture() const
{
    return m_textureID;
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

const glm::vec3& MeshData::Position(int index) const
{
    return m_instances[index].position;
}

const glm::vec3& MeshData::Scale(int index) const
{
    return m_instances[index].scale;
}

void MeshData::SetWorld(const glm::mat4& world, int index)
{
    m_instances[index].requiresUpdate = false;
    m_instances[index].world = world;
}

void MeshData::SetShouldRender(bool render, int index)
{
    m_instances[index].render = true;
}

void MeshData::Tick(const glm::vec3& cameraPosition)
{
    for (auto& instance : m_instances)
    {
        if (!instance.render)
        {
            continue;
        }

        if (instance.requiresUpdate)
        {
            instance.requiresUpdate = false;

            if (instance.rotation.x == 0 &&
                instance.rotation.y == 0 &&
                instance.rotation.z == 0)
            {
                instance.world[0][0] = instance.scale.x;
                instance.world[0][1] = 0.0f;
                instance.world[0][2] = 0.0f;
                instance.world[1][0] = 0.0f;
                instance.world[1][1] = instance.scale.y;
                instance.world[1][2] = 0.0f;
                instance.world[2][0] = 0.0f;
                instance.world[2][1] = 0.0f;
                instance.world[2][2] = instance.scale.z;
                instance.world[3][0] = instance.position.x;
                instance.world[3][1] = instance.position.y;
                instance.world[3][2] = instance.position.z;
            }
            else
            {
                glm::mat4 scale;
                scale[0][0] = instance.scale.x;
                scale[1][1] = instance.scale.y;
                scale[2][2] = instance.scale.z;

                glm::mat4 translate;
                translate[3][0] = instance.position.x;
                translate[3][1] = instance.position.y;
                translate[3][2] = instance.position.z;

                glm::mat4 rotateX, rotateY, rotateZ;
                rotateX = glm::rotate(rotateX, instance.rotation.x, glm::vec3(1, 0, 0));
                rotateY = glm::rotate(rotateY, instance.rotation.y, glm::vec3(0, 1, 0));
                rotateZ = glm::rotate(rotateZ, instance.rotation.z, glm::vec3(0, 0, 1));

                instance.world = translate * (rotateZ * rotateX * rotateY) * scale;
            }
        }
    }
}

std::vector<glm::vec3> MeshData::VertexPositions() const
{
    const int size = static_cast<int>(m_vertices.size());

    std::vector<glm::vec3> vertices;
    vertices.resize(size / m_vertexComponentCount);

    for (int i = 0, j = 0; i < size; i += m_vertexComponentCount, ++j)
    {
        vertices[j].x = m_vertices[i];    // x
        vertices[j].y = m_vertices[i+1];  // y
        vertices[j].z = m_vertices[i+2];  // z
    }

    return vertices;
}