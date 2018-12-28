////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SceneDataIDs.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Avaliable meshes in the game
*/
namespace MeshID
{
    enum ID
    {
        BULLET,
        TANK,
        TANKGUN,
        GROUND,
        WALL,
        WALLBOX,
        TANKP1,
        TANKP2,
        TANKP3,
        TANKP4,
        MAX
    };
}

/**
* Avaliables convex hull meshes in the game
*/
namespace HullID
{
    enum ID
    {
        GROUND,
        WALL,
        TANK,
        GUN,
        TANKP1,
        TANKP2,
        TANKP3,
        TANKP4,
        BULLET,
        MAX
    };
}

/**
* Available shaders to use
*/
namespace ShaderID
{
    enum ID
    {
        TOON,
        NORMAL,
        PROXY,
        SHADOW,
        SPRITE,
        POST,
        MAX
    };
}

/**
* Available lights to use
*/
namespace LightID
{
    enum ID
    {
        MAIN,
        MAX
    };
}

/**
* Collision Shapes avaliable for rigid bodies
*/
namespace ShapeID
{
    enum ID
    {
        GROUND,
        WALL,
        TANK,
        GUN,
        BULLET,
        TANKP1,
        TANKP2,
        TANKP3,
        TANKP4,
        MAX
    };
}


/**
* Textures avaliable
*/
namespace TextureID
{
    enum ID
    {
        BACKDROP,
        BORDER,
        BOX,
        BUTTON_HIGH,
        BUTTON_LOW,
        DIFF_HIGH,
        DIFF_EASY,
        DIFF_HARD,
        DIFF_MED,
        FLIP,
        GAME_OVER,
        GAME_OVER_MENU,
        GAME_OVER_REPLAY,
        GAME_OVER_P1,
        GROUND,
        TANK_GUN,
        TANK_BODY,
        TANK_NPC_GUN,
        TANK_NPC_BODY,
        TOON_TEXT,
        WALL,
        BULLET,
        HEALTH_BACK,
        HEALTH_BAR,
        HEALTH_STAR,
        MENU,
        PIXEL,
        MAX
    };
}

/**
* The amount of instances of each mesh
*/
namespace Instance
{
    enum Amount
    {
        GROUND = 1,
        TANKS = 6,
        BULLETS = 50,
        WALLS = 4,
        ENEMIES = 5
    };

    enum ID
    {
        PLAYER = Amount::ENEMIES
    };
}