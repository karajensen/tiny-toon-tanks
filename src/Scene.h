////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Postprocessing.h"

class Camera;
class PhysicsEngine;
class Tweaker;
struct SceneData;

/**
* Manager and owner of all objects
*/
class Scene
{
public:

    /**
    * Constructor
    */
    Scene();

    /**
    * Destructor
    */
    ~Scene();

    /**
    * Ticks the scene
    */
    void Tick();

    /**
    * Initialises the scene
    * @param physics The physics engine
    * @return whether initialisation was successful
    */
    bool Initialise(PhysicsEngine& physics);

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    * @param reset Callback to reset the tweak bar
    */
    void AddToTweaker(Tweaker& tweaker, std::function<void(void)> reset);

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(PostProcessing::Map map);

    /**
    * @return the data for the scene
    */
    const SceneData& GetSceneData() const;

    /**
    * @return the data for the scene
    */
    SceneData& GetSceneData();

private:

    /**
    * Prevent copying
    */
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

private:

    std::unique_ptr<SceneData> m_data;  ///< Elements of the scene
    int m_selectedLight = 0;            ///< Currently selected light in the tweak bar
    int m_selectedMesh = 0;             ///< Currently selected mesh in the tweak bar
    int m_selectedHull = 0;             ///< Currently selected hull in the tweak bar
}; 
