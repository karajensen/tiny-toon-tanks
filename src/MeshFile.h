////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - MeshFile.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "mesh.h"

#include <string>
#include <vector>

/**
* Mesh loaded from file to be rendered in the scene
*/
class MeshFile : public Mesh
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param shaderID The ID of the shader to use
    */
    MeshFile(const std::string& name, int shaderID);

    /**
    * Initialises the mesh data buffer from an OBJ file
    * @param path The full path to the mesh file
    * @param requiresUVs whether this mesh requires UVs
    * @param requiresNormals Whether this mesh requires normals
    * @param instances The number of instances to create
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            bool requiresUVs,
                            bool requiresNormals,
                            int instances);

private:

    /**
    * Prevent copying
    */
    MeshFile(const MeshFile&) = delete;
    MeshFile& operator=(const MeshFile&) = delete;
};