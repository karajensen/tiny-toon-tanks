////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "glm/glm.hpp"

class Tweaker;

/**
* Base Mesh Information
*/
class MeshData
{
public:

    /**
    * Constructor
    * @param name The name of the data
    * @param shader The ID of the shader to use
    * @param shaderName The name of the shader to use
    */
    MeshData(const std::string& name, 
             const std::string& shaderName,
             int shaderID);

    /**
    * Destructor
    */
    virtual ~MeshData();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Ticks the mesh
    * @param cameraPosition The world position of the camera
    */
    void Tick(const glm::vec3& cameraPosition);

    /**
    * Initialises the buffers for the mesh
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * Pre-renders the mesh
    */
    void PreRender() const;

    /**
    * Renders the mesh
    */
    void Render() const;

    /**
    * @return The name of the mesh
    */
    const std::string& Name() const;

    /**
    * @return The ID of the shader to render with
    */
    int ShaderID() const;

    /**
    * @return The positions of the vertices of this mesh
    */
    std::vector<glm::vec3> VertexPositions() const;

    /**
    * @return The vertices constructing this mesh
    */
    const std::vector<float>& Vertices() const;

    /**
    * @return The indicies constructing this mesh
    */
    const std::vector<unsigned long>& Indices() const;

    /**
    * @return The ID for each texture type used
    */
    const std::vector<int>& TextureIDs() const;

    /**
    * @return Whether back facing polygons are culled
    */
    bool BackfaceCull() const;

    /**
    * Sets Whether back facing polygons are culled
    */
    void BackfaceCull(bool value);

    /**
    * Sets the ID of the texture to use
    * @param ID The ID of the texture to use
    */
    void SetTexture(int ID);

    /**
    * @return whether this mesh is visible or not
    */
    bool ShouldRender() const;

    /**
    * Sets whether this mesh should render
    */
    void SetShouldRender(bool render);

    /**
    * @return the position of the mesh
    */
    const glm::vec3& Position() const;

    /**
    * @return the scale of the mesh
    */
    const glm::vec3& Scale() const;

    /**
    * Explicitly set the world glm::mat4
    * @note will be overridden if individual components are set
    */
    void SetWorld(const glm::mat4& world);

protected:

    int m_vertexComponentCount = 0;         ///< Number of components that make up a vertex
    std::vector<float> m_vertices;          ///< Mesh Vertex information
    std::vector<unsigned long> m_indices;   ///< Mesh Index information

private:

    /**
    * Prevent copying
    */
    MeshData(const MeshData&) = delete;
    MeshData& operator=(const MeshData&) = delete;

    /**
    * Determines the radius surrounding this mesh
    * This is the based on the furthest vertex from the mesh center
    */
    void GenerateRadius();

    glm::mat4 m_world;                    ///< World glm::mat4
    glm::vec3 m_position;                 ///< Position offset
    glm::vec3 m_rotation;                 ///< Degrees rotated around each axis
    glm::vec3 m_scale;                    ///< Scaling of the mesh
    int m_textureID = -1;                 ///< ID for the diffuse texture
    bool m_render = true;                 ///< Whether to draw the mesh
    bool m_requiresUpdate = false;        ///< Whether to update the world glm::mat4
    bool m_backfacecull = true;           ///< Whether backface culling is enabled
    const std::string m_name;             ///< Name of the mesh
    int m_shaderIndex = -1;               ///< Unique Index of the mesh shader to use
    unsigned int m_vaoID = 0;             ///< An unique ID for Vertex Array Object (VAO)
    unsigned int m_vboID = 0;             ///< Unique ID for the Vertex Buffer Object (VBO)   
    unsigned int m_iboID = 0;             ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;           ///< Whether the vertex buffer object is initialised or not
    const std::string m_shaderName;       ///< Name of the shader to use
    float m_radius = 0.0f;                ///< The radius of the sphere surrounding the mesh
};