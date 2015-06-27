////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

struct SceneData;

/**
* Builds all objects and diagnostics for the scene
*/
class SceneBuilder
{
public:

    /**
    * Constructor
    * @param data All information for the scene
    */
    SceneBuilder(SceneData& data);

    /**
    * Destructor
    */
    ~SceneBuilder();

    /**
    * Initialises the scene
    * @return Whether the initialization was successful
    */
    bool Initialise();

private:

    /**
    * Prevent copying
    */
    SceneBuilder(const SceneBuilder&) = delete;
    SceneBuilder& operator=(const SceneBuilder&) = delete;

    /**
    * Initialises all shader constants
    * @return Whether the initialization was successful
    */
    bool InitialiseShaderConstants();

    /**
    * Initiliases all shaders
    * @return Whether the initialization was successful
    */
    bool InitialiseShaders();

    /**
    * Initialises the lighting for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting();

    /**
    * Initialises all textures required
    * @return Whether the initialization was successful
    */
    bool InitialiseTextures();

    /**
    * Initialises the meshes for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes();

    /**
    * Initialises the shadows for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseShadows();

    /**
    * Initialises the physics for the scene
    * @return Whether the initialization was successful
    */
    bool InitialisePhysics();

    /**
    * Initialises the sprites for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseSprites();

    SceneData& m_data; ///< The scene to build
};                     