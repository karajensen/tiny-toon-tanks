////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "glm/glm.hpp"

class Tweaker;

/**
* Base Mesh Information
*/
class MeshData
{
public:

    /**
    * Callbacks for rendering a mesh instance
    */
    typedef std::function<void(const glm::mat4&)> RenderInstance;

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        glm::mat4 world;                       ///< World matrix
        glm::vec3 position = glm::vec3(0,0,0); ///< Position offset
        glm::vec3 rotation = glm::vec3(0,0,0); ///< Degrees rotated around each axis
        glm::vec3 scale = glm::vec3(1,1,1);    ///< Scaling of the mesh
        bool render = true;                    ///< Whether to draw the mesh
        bool requiresUpdate = false;           ///< Whether to update the world matrix
    };

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
    */
    void Tick();

    /**
    * Initialises the buffers for the mesh
    * @param the number of instances of this mesh
    * @return whether initialisation was successful
    */
    bool Initialise(int instances = 1);

    /**
    * Pre-renders the mesh
    */
    void PreRender() const;

    /**
    * Renders the mesh
    */
    void Render() const;

    /**
    * Renders the mesh
    * @param renderInstance Callback to render a single mesh instance
    */
    void Render(RenderInstance renderInstance) const;

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
    * @return The ID for the texture used
    */
    int GetTexture() const;

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
    * @return the position of the mesh
    */
    const glm::vec3& Position(int index = 0) const;

    /**
    * Sets the position of the mesh
    */
    void Position(float x, float y, float z, int index = 0);

    /**
    * Sets the rotation of the mesh in degrees
    */
    void Rotation(float x, float y, float z, int index = 0);

    /**
    * @return the scale of the mesh
    */
    const glm::vec3& Scale(int index = 0) const;

    /**
    * Sets whether the mesh should render
    */
    void SetShouldRender(bool render, int index = 0);

    /**
    * Explicitly set the world glm::mat4
    * @note will be overridden if individual components are set
    */
    void SetWorld(const glm::mat4& world, int index = 0);

    /**
    * @return the amount of instances of this mesh
    */
    int Instances() const;

    /**
    * Sets whether this mesh is visible
    */
    void Visible(bool isVisible, int index = 0);

    /**
    * @return whether any instance is visible
    */
    bool IsVisible() const;

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

    int m_textureID = -1;                 ///< ID for the diffuse texture
    bool m_backfacecull = true;           ///< Whether backface culling is enabled
    const std::string m_name;             ///< Name of the mesh
    int m_shaderIndex = -1;               ///< Unique Index of the mesh shader to use
    unsigned int m_vaoID = 0;             ///< An unique ID for Vertex Array Object (VAO)
    unsigned int m_vboID = 0;             ///< Unique ID for the Vertex Buffer Object (VBO)   
    unsigned int m_iboID = 0;             ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;           ///< Whether the vertex buffer object is initialised or not
    const std::string m_shaderName;       ///< Name of the shader to use
    float m_radius = 0.0f;                ///< The radius of the sphere surrounding the mesh
    std::vector<Instance> m_instances;    ///< Instances of this mesh
};