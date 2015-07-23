////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - CollisionEvent.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Event structure for a collision between two rigid bodies
*/
struct CollisionEvent
{    
    /**
    * Checks for equality with another event
    * @param collision The event to check equality with
    * @return whether to two events are equal
    */
    bool operator==(const CollisionEvent& collision)
    {
        return collision.BodyA.RigidBodyID == BodyA.RigidBodyID && 
               collision.BodyB.RigidBodyID == BodyB.RigidBodyID;
    }

    /**
    * Data for a colliding rigid body
    */
    struct CollisionBody
    {
        int MeshID = 0;        ///< ID of the graphical mesh for rendering
        int MeshInstance = 0;  ///< Associated instance of the graphical mesh
        int RigidBodyID = 0;   ///< The index of the rigid body
    };

    CollisionBody BodyA;    ///< Collidable body 1
    CollisionBody BodyB;    ///< Collidable body 2
    bool Ignore = false;    ///< Whether to ignore this collision event or not
    bool Processed = false; ///< Whether the collision has been acknowledged and waiting resolution
    bool Resolved = false;  ///< Whether the collision has been resolved
};