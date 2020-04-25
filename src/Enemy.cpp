////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Enemy.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Enemy.h"
#include "GlmHelper.h"

const float TIME_TO_MOVEMENT_UPDATE = 500.0;
const float DISTANCE_TO_MOVE_FORWARD = 10.0f;
const float MAX_DISTANCE_TO_SHOOT = 20.0f;
const float MIN_DISTANCE_TO_SHOOT = 5.0f;

Enemy::Enemy(MeshGroup& tankmesh, int instance)
    : Tank(tankmesh, instance)
{  
}

void Enemy::AddToTweaker(Tweaker& tweaker)
{
    Tank::AddToTweaker(tweaker);
}

void Enemy::Update(float deltatime)
{
    Tank::Update(deltatime);

    if (!IsDropping())
    {
        m_aiTimePassed += deltatime;
        if (m_aiTimePassed >= TIME_TO_MOVEMENT_UPDATE)
        {
            m_aiTimePassed = 0.0f;
            CreateAIMovement();
        }
        else
        {
            m_movement = m_previousMovement;
            m_movement = m_movement & ~Tank::FIRE;
        }
    }
}

void Enemy::CreateAIMovement()
{
    const int direction = rand() % 6;
    const int rotation = rand() % 8;
    const int gunrotation = rand() % 8;
    const int shoot = rand() % 10;

    if (shoot == 0)
    {
        Fire();
    }

    if (direction == 0)
    {
        Move(true);
    }
    else if (direction == 1)
    {
        Move(false);
    }
    else if ((m_previousMovement & Tank::FORWARDS) == Tank::FORWARDS)
    {
        Move(true);
    }
    else if ((m_previousMovement & Tank::BACKWARDS) == Tank::BACKWARDS)
    {
        Move(false);
    }

    if (rotation == 0)
    {
        Rotate(true);
    }
    else if (rotation == 1)
    {
        Rotate(false);
    }
    else if (rotation > 1 && rotation < 4)
    {
        if ((m_previousMovement & Tank::ROTATE_LEFT) == Tank::ROTATE_LEFT)
        {
            Rotate(true);
        }
        else if ((m_previousMovement & Tank::ROTATE_RIGHT) == Tank::ROTATE_RIGHT)
        {
            Rotate(false);
        }
    }

    if (gunrotation == 0)
    {
        RotateGun(true);
    }
    else if (gunrotation == 1)
    {
        RotateGun(false);
    }
    else if (gunrotation > 1 && gunrotation < 4)
    {
        if ((m_previousMovement & Tank::GUN_LEFT) == Tank::GUN_LEFT)
        {
            RotateGun(true);
        }
        else if ((m_previousMovement & Tank::GUN_RIGHT) == Tank::GUN_RIGHT)
        {
            RotateGun(false);
        }
    }
}