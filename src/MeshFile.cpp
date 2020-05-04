////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - MeshFile.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "MeshFile.h"
#include "Tweaker.h"
#include "Logger.h"

#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

MeshFile::MeshFile(const std::string& name, int shaderID)
    : Mesh(name, shaderID)
{
}

bool MeshFile::InitialiseFromFile(const std::string& path, 
                                  bool requiresUVs,
                                  bool requiresNormals, 
                                  int instances)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace|
        aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|
        aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices|aiProcess_GenSmoothNormals|
        aiProcess_LimitBoneWeights|aiProcess_RemoveRedundantMaterials|aiProcess_OptimizeMeshes);

    if(!scene)
    {
        LogError("Assimp import error for mesh " + path + ": " + importer.GetErrorString());
        return false;
    }

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    // For each submesh
    m_vertexComponentCount = 1;
    bool generatedComponentCount = false;
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        const unsigned int indexOffset = m_vertices.size() / m_vertexComponentCount;

        if(!pMesh->HasPositions())
        {
            LogError(Name() + " requires positions for requested shader");
            return false;
        }
        if(requiresUVs && !pMesh->HasTextureCoords(0))
        {
            LogError(Name() + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            LogError(Name() + " requires normals for requested shader");
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            m_vertices.push_back(pMesh->mVertices[vert].x);
            m_vertices.push_back(pMesh->mVertices[vert].y);
            m_vertices.push_back(pMesh->mVertices[vert].z);
            componentCount = 3;

            if (requiresUVs)
            {
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].x);
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].y);
                componentCount += 2;
            }

            if (requiresNormals)
            {
                m_vertices.push_back(pMesh->mNormals[vert].x);
                m_vertices.push_back(pMesh->mNormals[vert].y);
                m_vertices.push_back(pMesh->mNormals[vert].z);
                componentCount += 3;
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != m_vertexComponentCount)
            {
                LogError("Assimp error for mesh " + path + ": " + 
                    std::to_string(componentCount) + " does not match " +
                    std::to_string(m_vertexComponentCount));
                return false;
            }
        }
        else
        {
            m_vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // For each face
        for(unsigned int face = 0; face < pMesh->mNumFaces; ++face)
        {
            aiFace *pFace = &pMesh->mFaces[face];
            if(pFace->mNumIndices != 3)
            {
                LogError("Assimp error for mesh " + path + ": not all faces are triangles");
                return false;
            }

            m_indices.push_back(indexOffset + pFace->mIndices[0]);
            m_indices.push_back(indexOffset + pFace->mIndices[1]);
            m_indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }

    LogInfo("Mesh: " + Name() + " created");
    return Mesh::Initialise(instances);
}