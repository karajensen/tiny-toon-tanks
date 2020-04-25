////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SceneData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DataIDs.h"
#include "Postprocessing.h"
#include "Shader.h"
#include "Light.h"
#include "Mesh.h"
#include "Texture.h"

#include <vector>
#include <memory>

/**
* Internal data for the scene
*/
struct SceneData
{
    std::unique_ptr<PostProcessing> post;
    std::vector<std::unique_ptr<Shader>> shaders;
    std::vector<std::unique_ptr<Light>> lights;
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Mesh>> hulls;
    std::vector<std::unique_ptr<Mesh>> effects;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<int> shapes;
};