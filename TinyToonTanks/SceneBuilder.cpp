////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SceneBuilder.h"
#include "SceneData.h"
#include "Shader.h"
#include "GameData.h"
#include "BulletPhysics.h"
#include "Common.h"

namespace
{
    /**
    * Resource file information
    */
    const std::string ASSETS_PATH(".//resources//");
}

SceneBuilder::SceneBuilder() = default;
SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise(SceneData& data, BulletPhysicsWorld& physics)
{
    return InitialiseLighting(data) &&
           InitialiseTextures(data) &&
           InitialiseShaderConstants(data) &&
           InitialiseShaders(data) &&
           InitialiseMeshes(data) &&
           InitialiseHulls(data, physics) &&
           InitialisePhysics(data, physics) &&
           InitialiseSprites(data);
}

bool SceneBuilder::InitialiseLighting(SceneData& data)
{
    data.lights.resize(LightID::MAX);
    data.lights[LightID::MAIN] = std::make_unique<Light>("Main Light");
    data.lights[LightID::MAIN]->Position(glm::vec3(50.0f, 50.0f, 0.0f));
    data.lights[LightID::MAIN]->Diffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    return true;
}

bool SceneBuilder::InitialiseShaderConstants(SceneData& data)
{
    Shader::ShaderConstants constants = 
    {
        std::make_pair("MAX_LIGHTS", std::to_string(LightID::MAX)),
        std::make_pair("WINDOW_WIDTH", std::to_string(WINDOW_WIDTH)),
        std::make_pair("WINDOW_HEIGHT", std::to_string(WINDOW_HEIGHT)),
        std::make_pair("SAMPLES", std::to_string(MULTISAMPLING_COUNT))
    };

    Shader::InitialiseConstants(constants);
    return true;
}

bool SceneBuilder::InitialiseShaders(SceneData& data)
{
    auto Initialise = [&data](std::string name, ShaderID::ID ID) -> bool
    {
        data.shaders[ID] = std::make_unique<Shader>(name, ASSETS_PATH + name);
        return data.shaders[ID]->Initialise();
    };

    bool success = true;
    data.shaders.resize(ShaderID::MAX);

    success &= Initialise("normal", ShaderID::NORMAL);
    success &= Initialise("proxy", ShaderID::PROXY);
    success &= Initialise("shadow", ShaderID::SHADOW);
    success &= Initialise("toon", ShaderID::TOON);
    success &= Initialise("sprite", ShaderID::SPRITE);
    success &= Initialise("post", ShaderID::POST);

    return success;
}

bool SceneBuilder::InitialiseTextures(SceneData& data)
{
    bool success = true;
    data.textures.resize(TextureID::MAX);

    auto Initialise = [&data](std::string name, TextureID::ID ID) -> bool
    {
        data.textures[ID] = std::make_unique<Texture>(
            name, ASSETS_PATH + name, Texture::LINEAR);
        return data.textures[ID]->Initialise();
    };

    success &= Initialise("backdrop.png", TextureID::BACKDROP);
    success &= Initialise("border.png", TextureID::BORDER);
    success &= Initialise("boxUV.png", TextureID::BOX);
    success &= Initialise("buthigh.png", TextureID::BUTTON_HIGH);
    success &= Initialise("butnorm.png", TextureID::BUTTON_LOW);
    success &= Initialise("diffhigh.png", TextureID::DIFF_HIGH);
    success &= Initialise("easy.png", TextureID::DIFF_EASY);
    success &= Initialise("flip1.png", TextureID::FLIP);
    success &= Initialise("gameover.png", TextureID::GAME_OVER);
    success &= Initialise("gomenu.png", TextureID::GAME_OVER_MENU);
    success &= Initialise("gop1.png", TextureID::GAME_OVER_P1);
    success &= Initialise("goreplay.png", TextureID::GAME_OVER_REPLAY);
    success &= Initialise("groundUV.png", TextureID::GROUND);
    success &= Initialise("gunp1.png", TextureID::TANK_GUN);
    success &= Initialise("gunpn.png", TextureID::TANK_NPC_GUN);
    success &= Initialise("hard.png", TextureID::DIFF_HARD);
    success &= Initialise("health1.png", TextureID::HEALTH_BAR);
    success &= Initialise("medium.png", TextureID::DIFF_MED);
    success &= Initialise("menu.png", TextureID::MENU);
    success &= Initialise("pixel.png", TextureID::PIXEL);
    success &= Initialise("player.png", TextureID::HEALTH_BACK);
    success &= Initialise("star1.png", TextureID::HEALTH_STAR);
    success &= Initialise("tankp1.png", TextureID::TANK_BODY);
    success &= Initialise("tankpn.png", TextureID::TANK_NPC_BODY);
    success &= Initialise("toontext.png", TextureID::TOON_TEXT);
    success &= Initialise("wallUV.png", TextureID::WALL);

    return true;
}

bool SceneBuilder::InitialiseMeshes(SceneData& data)
{
    bool success = true;
    data.meshes.resize(MeshID::MAX);

    auto Initialise = [&data](std::string name, MeshID::ID meshID, ShaderID::ID shaderID) -> bool
    {
        data.meshes[meshID] = std::make_unique<Mesh>(name,
            data.shaders[shaderID]->Name(), shaderID);

        return data.meshes[meshID]->InitialiseFromFile(
            ASSETS_PATH + name + ".obj", glm::vec2(0, 0), true, true);
    };

    success &= Initialise("bullet", MeshID::BULLET, ShaderID::TOON);
    success &= Initialise("tank", MeshID::TANK, ShaderID::TOON);
    success &= Initialise("tankgun", MeshID::TANKGUN, ShaderID::TOON);
    success &= Initialise("ground", MeshID::GROUND, ShaderID::TOON);
    success &= Initialise("wall", MeshID::WALL, ShaderID::TOON);
    success &= Initialise("wall", MeshID::WALL, ShaderID::TOON);
    success &= Initialise("world", MeshID::WORLD, ShaderID::SHADOW);
    success &= Initialise("tankshadow", MeshID::TANKSHADOW, ShaderID::SHADOW);
    success &= Initialise("tankp1", MeshID::TANKP1, ShaderID::TOON);
    success &= Initialise("tankp2", MeshID::TANKP2, ShaderID::TOON);
    success &= Initialise("tankp3", MeshID::TANKP3, ShaderID::TOON);
    success &= Initialise("tankp4", MeshID::TANKP4, ShaderID::TOON);

    return success;
}

bool SceneBuilder::InitialiseSprites(SceneData& data)
{
    return true;
}


bool SceneBuilder::InitialiseHulls(SceneData& data, BulletPhysicsWorld& physics)
{
    bool success = true;
    data.hulls.resize(HullID::MAX);
    m_shapeIDS.resize(ShapeID::MAX);

    auto Initialise = [this, &data, &physics](std::string name, HullID::ID hullID, 
                                              ShaderID::ID shaderID, ShapeID::ID shapeID) -> bool
    {
        auto& hull = data.hulls[hullID];
        hull = std::make_unique<Mesh>(name, data.shaders[shaderID]->Name(), shaderID);
        hull->SetShouldRender(false);

        if (hull->InitialiseFromFile(ASSETS_PATH + name + ".obj", glm::vec2(0, 0), false, false))
        {
            m_shapeIDS[shapeID] = physics.LoadConvexShape(hull->VertexPositions());
            return true;
        }
        return false;
    };

    success &= Initialise("tankp1proxy", HullID::TANKP1, ShaderID::PROXY, ShapeID::TANKP1);
    success &= Initialise("tankp2proxy", HullID::TANKP2, ShaderID::PROXY, ShapeID::TANKP2);
    success &= Initialise("tankp3proxy", HullID::TANKP3, ShaderID::PROXY, ShapeID::TANKP3);
    success &= Initialise("tankp4proxy", HullID::TANKP4, ShaderID::PROXY, ShapeID::TANKP4);
    success &= Initialise("tankproxy", HullID::TANK, ShaderID::PROXY, ShapeID::TANK);
    success &= Initialise("tankgunproxy", HullID::GUN, ShaderID::PROXY, ShapeID::GUN);
    success &= Initialise("bulletproxy", HullID::BULLET, ShaderID::PROXY, ShapeID::BULLET);
    success &= Initialise("groundproxy", HullID::GROUND, ShaderID::PROXY, ShapeID::GROUND);
    success &= Initialise("wallproxy", HullID::WALL, ShaderID::PROXY, ShapeID::WALL);

    return success;
}

bool SceneBuilder::InitialisePhysics(SceneData& data, BulletPhysicsWorld& physics)
{

    return true;
}