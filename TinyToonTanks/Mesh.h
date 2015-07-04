////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "meshdata.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shaderName The name of the shader to use
    * @param shaderID The ID of the shader to use
    */
    Mesh(const std::string& name, 
         const std::string& shaderName, 
         int shaderID);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Initialises the mesh data buffer from an OBJ file
    * @param path The full path to the mesh file
    * @param uvScale The scale to apply to mesh UVs
    * @param requiresUVs whether this mesh requires UVs
    * @param requiresNormals Whether this mesh requires normals
    * @param instances The number of instances to create
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            const glm::vec2& uvScale,
                            bool requiresUVs,
                            bool requiresNormals,
                            int instances);

private:

    /**
    * Prevent copying
    */
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};