////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postprocessing.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postprocessing.h"
#include "tweaker.h"

PostProcessing::PostProcessing()
{
    SetPostMap(FINAL_MAP);
}

void PostProcessing::AddToTweaker(Tweaker& tweaker)
{
}

std::string PostProcessing::GetPostMap() const
{
    return GetMapName(m_selectedMap);
}

void PostProcessing::SetPostMap(PostProcessing::Map map)
{
    m_selectedMap = map;
    m_masks.assign(0.0f);
    m_masks[map] = 1.0f;
}

float PostProcessing::Mask(PostProcessing::Map map) const
{
    return m_masks.at(map);
}

std::string PostProcessing::GetMapName(PostProcessing::Map map)
{
    switch (map)
    {
    case FINAL_MAP:
        return "Final Scene";
    case SCENE_MAP:
        return "Scene Map";
    case NORMAL_MAP:
        return "Normal Map";
    case TOONLINE_MAP:
        return "Toon Line Map";
    default:
        return "None";
    }
}