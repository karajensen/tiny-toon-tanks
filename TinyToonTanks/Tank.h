////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tank.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm\glm.hpp"
#include "DataIDs.h"

class Tweaker;
class Mesh;

/**
* Controllable tank
*/
class Tank
{
public:

    /**
    * IDs for the tank physics elements
    */
    struct PhysicsIDs
    {
        int Body = 0;
        int Gun = 0;
        int Hinge = 0;
        int P1 = 0;
        int P2 = 0;
        int P3 = 0;
        int P4 = 0;
    };

    /**
    * Flags for particular movement requests
    */
    enum MovementFlag
    {
        NO_MOVEMENT = 0,
        FORWARDS = 2,
        BACKWARDS = 4,
        ROTATE_LEFT = 8,
        ROTATE_RIGHT = 16,
        FLIP = 32,
        GUN_LEFT = 64,
        GUN_RIGHT = 128,
        FIRE = 256
    };

    /**
    * Holds individual pieces of the overall tank graphics mesh
    */
    struct MeshGroup
    {
        /**
        * Constructor
        */
        MeshGroup(Mesh& body,
                  Mesh& gun,
                  Mesh& p1,
                  Mesh& p2,
                  Mesh& p3,
                  Mesh& p4) :
            Body(body),
            Gun(gun),
            P1(p1),
            P2(p2),
            P3(p3),
            P4(p4)
        {
        }

        Mesh& Body; ///< Main body of the tank
        Mesh& Gun;  ///< Gun on top of the tank
        Mesh& P1;   ///< First breakable piece
        Mesh& P2;   ///< Second breakable piece
        Mesh& P3;   ///< Third breakable piece
        Mesh& P4;   ///< Forth breakable piece
    };

    /**
    * Constructor
    * @param mesh Tankmesh Holds each piece of the tank
    * @param instance Which instance this tank is to access the mesh parts
    */
    Tank(MeshGroup& tankmesh, int instance);

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
    * Updates the tank
    * @param deltatime The time passed between ticks in seconds
    */
    void Update(float deltatime);

    /**
    * @return the position of the tank
    */
    const glm::vec3& GetPosition() const;

    /**
    * @return the world matrix for the tank
    */
    const glm::mat4& GetWorldMatrix() const;

    /**
    * Set the world matrix for the tank piece
    */
    void SetPieceWorldMatrix(MeshID::ID id, const glm::mat4& matrix);

    /**
    * @return the world matrix for the gun
    */
    const glm::mat4& GetGunWorldMatrix() const;

    /**
    * Sets physics IDs for this tank
    */
    void SetPhysicsIDs(PhysicsIDs IDs);

    /**
    * @return the physics IDs for this tank
    */
    const PhysicsIDs& GetPhysicsIDs() const;

    /**
    * @return whether this tank is alive
    */
    bool IsAlive() const;

    /**
    * Sets whether this tank is alive
    */
    void SetIsAlive(bool alive);
    
    /**
    * @return the instance of the graphics mesh
    */
    int GetInstance() const;

    /**
    * Sends a request to fire the gun
    * @note requests are time restricted
    */
    void Fire();

    /**
    * Sends a request to flip over
    */
    void Flip();

    /**
    * Sends a request to rotate the tank
    * @param left Whether to rotate left or right
    */
    void Rotate(bool left);

    /**
    * Sends a request to rotate the tank gun
    * @param left Whether to rotate left or right
    */
    void RotateGun(bool left);

    /**
    * Sends a request to move the tank
    * @param forwards Whether to move forwards or backwards
    */
    void Move(bool forwards);

    /**
    * @return the type of movement requests
    */
    unsigned int GetMovementRequest() const;

    /**
    * Resets all movement requests
    */
    void ResetMovementRequest();

    /**
    * Sets the linear damping for the tank movement force
    */
    void SetLinearDamping(float value);

    /**
    * Adds linear damping for the tank movement force
    */
    void AddLinearDamping(float value);

    /**
    * @return the linear damping for the tank movement force
    */
    float GetLinearDamping() const;

    /**
    * Sets the rotational damping for the tank rotation
    */
    void SetRotationalDamping(float value);

    /**
    * Adds rotational damping for the tank rotation force
    */
    void AddRotationalDamping(float value);

    /**
    * @return the rotational damping for the tank rotation force
    */
    float GetRotationalDamping() const;

    /**
    * Sets the damping for the gun rotation movement
    */
    void SetGunRotationalDamping(float value);

    /**
    * Adds damping for the gun rotation movement
    */
    void AddGunRotationalDamping(float value);

    /**
    * @return damping for the gun rotation movement
    */
    float GetGunRotationalDamping() const;

    /**
    * Resets the tank
    */
    void Reset();

    /**
    * Set whether the tank is dropping to the ground when first spawned
    */
    void SetDropping(bool dropping);

    /**
    * @return whether the tank is dropping to the ground when first spawned
    */
    bool IsDropping() const;

    /**
    * Reduces the life of the tank
    */
    void TakeDamage(int amount);

    /**
    * @return the health of this tank
    */
    int Health() const;

protected:

    /**
    * Prevent copying
    */
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;

    PhysicsIDs m_physicsIDs;                ///< IDs for the physics elements
    MeshGroup& m_tankmesh;                  ///< Holds each piece of the tank
    int m_instance = 0;                     ///< Which instance this tank is to access the mesh parts
    bool m_alive = true;                    ///< Whether this tank is alive
    unsigned int m_movement = NO_MOVEMENT;  ///< Requests for movement
    float m_linearDamping = 1.0f;           ///< Damping value for linear movement
    float m_rotationalDamping = 1.0f;       ///< Damping value for rotational movement
    float m_gunDamping = 1.0f;              ///< Damping value for the gun rotation
    float m_fireGunTime = 0.0f;             ///< Time passed since a fire gun request
    bool m_isDropping = true;               ///< Whether the tank is dropping to the ground when first spawned
    int m_health = 0;                       ///< The health of this tank
};