////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Postprocessing.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>

class Tweaker;

/**
* Holds data for post processing the final image
*/
class PostProcessing
{
public:

    /**
    * Post Shader maps
    */
    enum Map
    {
        FINAL_MAP,
        SCENE_MAP,
        NORMAL_MAP,
        TOONLINE_MAP,
        MAX_MAPS
    };

    /**
    * Constructor
    */
    PostProcessing();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    void AddToTweaker(Tweaker& tweaker);

    /**
    * Sets which post map should be rendered
    * @param map The post map to render
    */
    void SetPostMap(Map map);

    /**
    * @return The mask value for the map
    */
    float Mask(PostProcessing::Map map) const;

    /**
    * @return the currently rendered map
    */
    std::string GetPostMap() const;

    /**
    * @param map the map type to convert
    * @return The string name of the map type
    */
    static std::string GetMapName(Map map);

private:

    /**
    * Prevent copying
    */
    PostProcessing(const PostProcessing&) = delete;
    PostProcessing& operator=(const PostProcessing&) = delete;

private:

    std::array<float, MAX_MAPS> m_masks;  ///< Visibility of post maps
    Map m_selectedMap;                    ///< The currently rendered post map
};