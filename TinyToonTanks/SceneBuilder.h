////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

class BulletPhysicsWorld;
struct SceneData;

/**
* Builds all objects and diagnostics for the scene
*/
class SceneBuilder
{
public:

    SceneBuilder();
    ~SceneBuilder();

    /**
    * Initialises the scene
    * @param data All information for the scene
    * @param physics The physics engine
    * @return Whether the initialization was successful
    */
    bool Initialise(SceneData& data, BulletPhysicsWorld& physics);

private:

    /**
    * Prevent copying
    */
    SceneBuilder(const SceneBuilder&) = delete;
    SceneBuilder& operator=(const SceneBuilder&) = delete;

    /**
    * Initialises all shader constants
    * @param data All information for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseShaderConstants(SceneData& data);

    /**
    * Initiliases all shaders
    * @param data All information for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseShaders(SceneData& data);

    /**
    * Initialises the lighting for the scene
    * @param data All information for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseLighting(SceneData& data);

    /**
    * Initialises all textures required
    * @param data All information for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseTextures(SceneData& data);

    /**
    * Initialises the meshes for the scene
    * @param data All information for the scene
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseMeshes(SceneData& data);

    /**
    * Initialises the convex hulls for the scene
    * @param data All information for the scene
    * @param physics The physics world
    * @note relies on shaders initialised before
    * @return Whether the initialization was successful
    */
    bool InitialiseHulls(SceneData& data, BulletPhysicsWorld& physics);

    /**
    * Initialises the physics for the scene
    * @param data All information for the scene
    * @param physics The physics world
    * @return Whether the initialization was successful
    */
    bool InitialisePhysics(SceneData& data, BulletPhysicsWorld& physics);

    /**
    * Initialises the sprites for the scene
    * @param data All information for the scene
    * @return Whether the initialization was successful
    */
    bool InitialiseSprites(SceneData& data);

    std::vector<int> m_shapeIDS;  ///< IDS for the physics collision shapes
};                     