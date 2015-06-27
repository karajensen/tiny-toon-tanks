////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SceneData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

#include "Shader.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    SceneData()
    {
    }

    ~SceneData()
    {
    }

    std::vector<std::unique_ptr<Shader>> shaders;
};