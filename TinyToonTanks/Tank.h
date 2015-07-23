////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include "TankData.h"

class Tweaker;
class Mesh;

/**
* Controllable tank
*/
class Tank
{
public:

    /**
    * Constructor
    * @param mesh Tankmesh Holds each piece of the tank
    * @param instance Which instance this tank is to access the mesh parts
    */
    Tank(TankMesh& tankmesh, int instance);

    /**
    * Destructor
    */
    virtual ~Tank();

    /**
    * Adds data for this element to be tweaked by the gui
    * @param tweaker The helper for adding tweakable entries
    */
    virtual void AddToTweaker(Tweaker& tweaker);

    /**
    * @return the position of the tank
    */
    const glm::vec3& GetPosition() const;

    /**
    * Sets physics IDs for this tank
    */
    void SetPhysicsIDs(TankPhysicsIDs IDs);

    /**
    * @return the physics IDs for this tank
    */
    const TankPhysicsIDs& GetPhysicsIDs() const;

    /**
    * @return whether this tank is alive
    */
    bool IsAlive() const;
    
    /**
    * @return the instance of the graphics mesh
    */
    int GetInstance() const;

protected:

    /**
    * Prevent copying
    */
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;

    TankPhysicsIDs m_physicsIDs; ///< IDs for the physics elements
    TankMesh& m_tankmesh;        ///< Holds each piece of the tank
    int m_instance = 0;          ///< Which instance this tank is to access the mesh parts
    bool m_alive = true;         ///< Whether this tank is alive
};