////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SceneBuilder.h"
#include "SceneData.h"
#include "Shader.h"
#include "Common.h"
#include <assert.h>

namespace
{
    /**
    * Resource file information
    */
    const std::string ASSETS_PATH(".//resources//");
}

SceneBuilder::SceneBuilder(SceneData& data) :
    m_data(data)
{
}

SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise()
{
    return InitialiseLighting() &&
           InitialiseTextures() &&
           InitialiseShaderConstants() &&
           InitialiseShaders() &&
           InitialiseMeshes() &&
           InitialiseShadows() &&
           InitialisePhysics() &&
           InitialiseSprites();
}

bool SceneBuilder::InitialiseLighting()
{
    return true;
}

bool SceneBuilder::InitialiseShaderConstants()
{
    Shader::ShaderConstants constants = 
    {
        std::make_pair("WINDOW_WIDTH", std::to_string(WINDOW_WIDTH)),
        std::make_pair("WINDOW_HEIGHT", std::to_string(WINDOW_HEIGHT)),
        std::make_pair("SAMPLES", std::to_string(MULTISAMPLING_COUNT))
    };

    Shader::InitialiseConstants(constants);
    return true;
}

bool SceneBuilder::InitialiseShaders()
{
    return true;
}

bool SceneBuilder::InitialiseTextures()
{
    return true;
}

bool SceneBuilder::InitialiseShadows()
{
    return true;
}

bool SceneBuilder::InitialiseMeshes()
{
    return true;
}

bool SceneBuilder::InitialiseSprites()
{
    return true;
}

bool SceneBuilder::InitialisePhysics()
{
    return true;
}