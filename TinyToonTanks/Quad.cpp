////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - quad.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "quad.h"

Quad::Quad(const std::string& name, int shader) :
    MeshData(name, "unnamed", shader)
{
    m_vertexComponentCount = 5;

    // Top left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(-1.0f);  // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(0.0f);  // v

    // Top right corner
    m_vertices.emplace_back(1.0f); // x
    m_vertices.emplace_back(-1.0f); // y
    m_vertices.emplace_back(0.0f); // z
    m_vertices.emplace_back(1.0f); // u
    m_vertices.emplace_back(0.0f); // v

    // Bot right corner
    m_vertices.emplace_back(1.0f);  // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(1.0f);  // u
    m_vertices.emplace_back(1.0f); // v

    // Bot left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(1.0f); // v

    m_indices.emplace_back(0);
    m_indices.emplace_back(3);
    m_indices.emplace_back(1);
              
    m_indices.emplace_back(1);
    m_indices.emplace_back(3);
    m_indices.emplace_back(2);
}
