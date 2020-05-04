////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SceneBuilder.h"
#include "SceneData.h"
#include "Shader.h"
#include "PhysicsEngine.h"
#include "ToonText.h"
#include "MeshFile.h"
#include "Utils.h"

namespace
{
    /**
    * Resource file information
    */
    const std::string ASSETS_PATH(".//resources//");
}

SceneBuilder::SceneBuilder() = default;
SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise(SceneData& data, PhysicsEngine& physics)
{
    data.post = std::make_unique<PostProcessing>();
    return InitialiseLighting(data) &&
           InitialiseTextures(data) &&
           InitialiseShaderConstants(data) &&
           InitialiseShaders(data) &&
           InitialiseMeshes(data) &&
           InitialiseEffects(data) &&
           InitialiseHulls(data, physics);
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
        std::make_pair("SAMPLES", std::to_string(MULTISAMPLING_COUNT)),
        std::make_pair("SCENE_TEXTURES", std::to_string(SCENE_TEXTURES)),
        std::make_pair("ID_COLOUR", std::to_string(ID_COLOUR)),
        std::make_pair("ID_NORMAL", std::to_string(ID_NORMAL))
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

    success &= Initialise("proxy", ShaderID::PROXY);
    success &= Initialise("shadow", ShaderID::SHADOW);
    success &= Initialise("toon", ShaderID::TOON);
    success &= Initialise("texture", ShaderID::TEXTURE);
    success &= Initialise("post", ShaderID::POST);
    success &= Initialise("gradient", ShaderID::GRADIENT);

    return success;
}

bool SceneBuilder::InitialiseTextures(SceneData& data)
{
    bool success = true;
    data.textures.resize(TextureID::MAX);

    auto Initialise = [&data](std::string name, TextureID::ID ID, Texture::Filter filter) -> bool
    {
        data.textures[ID] = std::make_unique<Texture>(
            name, ASSETS_PATH + name, filter);
        return data.textures[ID]->Initialise();
    };

    success &= Initialise("border.png", TextureID::BORDER, Texture::NEAREST);
    success &= Initialise("boxUV.png", TextureID::BOX, Texture::ANISOTROPIC);
    success &= Initialise("buthigh.png", TextureID::BUTTON_HIGH, Texture::NEAREST);
    success &= Initialise("butnorm.png", TextureID::BUTTON_LOW, Texture::NEAREST);
    success &= Initialise("diffhigh.png", TextureID::DIFF_HIGH, Texture::NEAREST);
    success &= Initialise("easy.png", TextureID::DIFF_EASY, Texture::NEAREST);
    success &= Initialise("gameover.png", TextureID::GAME_OVER, Texture::NEAREST);
    success &= Initialise("gomenu.png", TextureID::GAME_OVER_MENU, Texture::NEAREST);
    success &= Initialise("gop1.png", TextureID::GAME_OVER_P1, Texture::NEAREST);
    success &= Initialise("goreplay.png", TextureID::GAME_OVER_REPLAY, Texture::NEAREST);
    success &= Initialise("groundUV.png", TextureID::GROUND, Texture::ANISOTROPIC);
    success &= Initialise("gunp1.png", TextureID::TANK_GUN, Texture::ANISOTROPIC);
    success &= Initialise("gunpn.png", TextureID::TANK_NPC_GUN, Texture::ANISOTROPIC);
    success &= Initialise("hard.png", TextureID::DIFF_HARD, Texture::NEAREST);
    success &= Initialise("health1.png", TextureID::HEALTH_BAR, Texture::NEAREST);
    success &= Initialise("medium.png", TextureID::DIFF_MED, Texture::NEAREST);
    success &= Initialise("menu.png", TextureID::MENU, Texture::NEAREST);
    success &= Initialise("pixel.png", TextureID::PIXEL, Texture::NEAREST);
    success &= Initialise("player.png", TextureID::HEALTH_BACK, Texture::NEAREST);
    success &= Initialise("star1.png", TextureID::HEALTH_STAR, Texture::NEAREST);
    success &= Initialise("tankp1.png", TextureID::TANK_BODY, Texture::ANISOTROPIC);
    success &= Initialise("tankpn.png", TextureID::TANK_NPC_BODY, Texture::ANISOTROPIC);
    success &= Initialise("toontext.png", TextureID::TOON_TEXT, Texture::ANISOTROPIC);
    success &= Initialise("wallUV.png", TextureID::WALL, Texture::ANISOTROPIC);
    success &= Initialise("bullet.png", TextureID::BULLET, Texture::NEAREST);

    return true;
}

bool SceneBuilder::InitialiseEffects(SceneData& data)
{
    bool success = true;

    data.effects.resize(EffectID::MAX);

    // Initialise the toon text
    const int toonTextCount = 10;
    auto toonText = std::make_unique<ToonText>("toontext", ShaderID::TEXTURE);
    toonText->SetBackfaceCull(false);
    toonText->SetDepthWrite(false);
    toonText->SetAlphaBlending(true);
    success &= toonText->Initialise(toonTextCount);
    for (int i = 0; i < toonTextCount; ++i)
    {
        toonText->SetTexture(TextureID::TOON_TEXT, i);
        toonText->SetVisible(false, i);
    }
    data.effects[EffectID::TOONTEXT] = std::move(toonText);

    return success;
}

bool SceneBuilder::InitialiseMeshes(SceneData& data)
{
    bool success = true;
    const int NO_TEXTURE = -1;
    data.meshes.resize(MeshID::MAX);

    auto Initialise = [&data, NO_TEXTURE](const std::string& name, 
                                          int meshID, 
                                          int shaderID, 
                                          int textureID, 
                                          int instances, 
                                          bool shadows) -> bool
    {
        auto mesh = std::make_unique<MeshFile>(name, shaderID);
        mesh->SetRenderShadows(shadows);
        if (mesh->InitialiseFromFile(
            ASSETS_PATH + name + ".obj", true, true, instances))
        {
            if (textureID != NO_TEXTURE)
            {
                for (int i = 0; i < instances; ++i)
                {
                    mesh->SetTexture(textureID, i);
                }            
            }
            data.meshes[meshID] = std::move(mesh);
            return true;
        }
        return false;
    };

    success &= Initialise("bullet", MeshID::BULLET, ShaderID::TOON, TextureID::BULLET, Instance::BULLETS, true);
    success &= Initialise("tank", MeshID::TANK, ShaderID::TOON, TextureID::TANK_NPC_BODY, Instance::TANKS, true);
    success &= Initialise("tankgun", MeshID::TANKGUN, ShaderID::TOON, TextureID::TANK_NPC_GUN, Instance::TANKS, true);
    success &= Initialise("ground", MeshID::GROUND, ShaderID::TOON, TextureID::GROUND, Instance::GROUND, false);
    success &= Initialise("wall", MeshID::WALL, ShaderID::TOON, TextureID::WALL, Instance::WALLS, false);
    success &= Initialise("wallbox", MeshID::WALLBOX, ShaderID::TOON, TextureID::BOX, Instance::WALLS, false);
    success &= Initialise("tankp1", MeshID::TANKP1, ShaderID::TOON, TextureID::TANK_NPC_BODY, Instance::TANKS, true);
    success &= Initialise("tankp2", MeshID::TANKP2, ShaderID::TOON, TextureID::TANK_NPC_BODY, Instance::TANKS, true);
    success &= Initialise("tankp3", MeshID::TANKP3, ShaderID::TOON, TextureID::TANK_NPC_BODY, Instance::TANKS, true);
    success &= Initialise("tankp4", MeshID::TANKP4, ShaderID::TOON, TextureID::TANK_NPC_GUN, Instance::TANKS, true);

    // Initialise the backdrop
    data.meshes[MeshID::BACKDROP] = std::make_unique<Quad>("backdrop", ShaderID::GRADIENT);
    success &= data.meshes[MeshID::BACKDROP]->Initialise();
    data.meshes[MeshID::BACKDROP]->SetBackfaceCull(false);
    data.meshes[MeshID::BACKDROP]->SetAlphaBlending(true);
    data.meshes[MeshID::BACKDROP]->SetDepthWrite(false);
    data.meshes[MeshID::BACKDROP]->SetRenderWithLights(false);

    // Player has different texture to enemies
    data.meshes[MeshID::TANK]->SetTexture(TextureID::TANK_BODY, Instance::PLAYER);
    data.meshes[MeshID::TANKGUN]->SetTexture(TextureID::TANK_GUN, Instance::PLAYER);
    data.meshes[MeshID::TANKP1]->SetTexture(TextureID::TANK_BODY, Instance::PLAYER);
    data.meshes[MeshID::TANKP2]->SetTexture(TextureID::TANK_BODY, Instance::PLAYER);
    data.meshes[MeshID::TANKP3]->SetTexture(TextureID::TANK_BODY, Instance::PLAYER);
    data.meshes[MeshID::TANKP4]->SetTexture(TextureID::TANK_GUN, Instance::PLAYER);

    return success;
}

bool SceneBuilder::InitialiseHulls(SceneData& data, PhysicsEngine& physics)
{
    bool success = true;
    data.hulls.resize(HullID::MAX);
    data.shapes.resize(ShapeID::MAX);

    auto Initialise = [&data, &physics](std::string name, HullID::ID hullID, 
                                        ShaderID::ID shaderID, ShapeID::ID shapeID, 
                                        int instances) -> bool
    {
        auto hull = std::make_unique<MeshFile>(name, shaderID);
        if (hull->InitialiseFromFile(ASSETS_PATH + name + ".obj", false, false, instances))
        {
            hull->SetShouldRender(false);
            data.shapes[shapeID] = physics.LoadConvexShape(hull->VertexPositions());
            data.hulls[hullID] = std::move(hull);
            return true;
        }
        return false;
    };

    success &= Initialise("tankp1proxy", HullID::TANKP1, ShaderID::PROXY, ShapeID::TANKP1, Instance::TANKS);
    success &= Initialise("tankp2proxy", HullID::TANKP2, ShaderID::PROXY, ShapeID::TANKP2, Instance::TANKS);
    success &= Initialise("tankp3proxy", HullID::TANKP3, ShaderID::PROXY, ShapeID::TANKP3, Instance::TANKS);
    success &= Initialise("tankp4proxy", HullID::TANKP4, ShaderID::PROXY, ShapeID::TANKP4, Instance::TANKS);
    success &= Initialise("tankproxy", HullID::TANK, ShaderID::PROXY, ShapeID::TANK, Instance::TANKS);
    success &= Initialise("tankgunproxy", HullID::GUN, ShaderID::PROXY, ShapeID::GUN, Instance::TANKS);
    success &= Initialise("bulletproxy", HullID::BULLET, ShaderID::PROXY, ShapeID::BULLET, Instance::BULLETS);
    success &= Initialise("groundproxy", HullID::GROUND, ShaderID::PROXY, ShapeID::GROUND, Instance::GROUND);
    success &= Initialise("wallproxy", HullID::WALL, ShaderID::PROXY, ShapeID::WALL, Instance::WALLS);

    return success;
}