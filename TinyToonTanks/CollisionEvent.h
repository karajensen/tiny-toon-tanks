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
    * @param colEvent The event to check equality with
    * @return whether to two events are equal
    */
    bool operator==(const CollisionEvent& colEvent)
    {
        return colEvent.body1 == body1 && colEvent.body2 == body2;
    }

    int shape1 = 0;         ///< The shape of rigid body no.1
    int shape2 = 0;         ///< The shape of rigid body no.2
    int body1 = 0;          ///< The internal index of the rigid body no.1
    int body2 = 0;          ///< The internal index of the rigid body no.2
    int objindex1 = 0;      ///< The user defined index of rigid body no.1
    int objindex2 = 0;      ///< The user defined index of rigid body no.2
    bool ignore = false;    ///< Whether to ignore this collision event or not
    bool processed = false; ///< Whether the collision has been acknowledged and waiting resolution
    bool resolved = false;  ///< Whether the collision has been resolved
};