////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletPhysics.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "bullet/include/btBulletCollisionCommon.h"
#include "bullet/include/btBulletDynamicsCommon.h"
#include "bullet/include/linearMath/btTransform.h"

struct MeshKey;
struct RigidBody;
struct CollisionEvent;

/**
* Holds the Bullet Physics world for Tiny Toon Tanks
*/
class BulletPhysicsWorld
{
public:

    /**
    * Constructor
    */
    BulletPhysicsWorld();

    /**
    * Destructor
    */
    ~BulletPhysicsWorld();

    /**
    * Tick the simulation
    * @Param timestep How much to proceed the physics per iteration
    */
    void Tick(float timestep);

    /**
    * Generates a collision event
    * @param collisionIndex The index from currently occuring collisions
    * @param collision The collision event to be filled in
    * @return whether generation was successful
    */
    bool GenerateCollisionEvent(int collisionIndex, CollisionEvent& collision);

    /**
    * @return the number of collisions currently occuring
    */
    int GetCollisionAmount() const;
    
    /**
    * @param rigidBody The index for the rigid body
    * @return the friction of the given rigid body
    */
    float GetFriction(int rigidBodyID) const;

    /**
    * Set the collision group for the rigid body
    * @param rigidBody The index for the rigid body
    * @param group The collision group the rigid body belongs to
    */
    void SetGroup(int rigidBodyID, int group);

    /**
    * Get the current velocity of a rigid body
    * @param rigidBody The index for the rigid body
    * @return The velocity of the rigid body
    */
    glm::vec3 GetVelocity(int rigidBodyID) const;

    /**
    * Get the current transform of a rigid body
    * @param rigidBody The index for the rigid body
    * @return the transform of the right body
    */
    glm::mat4 GetTransform(int rigidBodyID) const;

    /**
    * Reset the simulation world
    */
    void ResetSimulation();

    /**
    * Explicitly sets the transform for a rigid body
    * @param rigidBody The rigid body to set
    * @param matrix The transform to set
    */
    void SetMotionState(int rigidBodyID, const glm::mat4& matrix);

    /**
    * Explicitly sets the position for a rigid body
    * @param rigidBody The rigid body to set
    * @param position The position to set
    */
    void SetPosition(int rigidBodyID, const glm::vec3& position);

    /**
    * Explicitly sets the basis for a rigid body
    * @param rigidBody The rigid body to set
    * @param mat The basis transform to set
    */
    void SetBasis(int rigidBodyID, const glm::mat4& matrix);

    /**
    * Sets the friction for a rigid body
    * @param rigidbody The rigid body to to set
    * @param amount The amount of friction to set
    */
    void SetFriction(int rigidBodyID, float amount);

    /**
    * Sets the gravity for a rigid body
    * @param rigidbody The rigid body to to set
    * @param gravity The amount of gravity to set
    */
    void SetGravity(int rigidBodyID, float gravity);

    /**
    * Sets the velocity of a rigid body
    * @param velocity The velocity to set
    * @param rigidbody The index for the rigid body to set
    * @param linearDamping The damping for linear velocity
    * @param angularDamping The damping for angular velocity
    */
    void SetVelocity(const glm::vec3& velocity, 
                     int rigidBodyID, 
                     float linearDamping = 0, 
                     float angularDamping = 0);

    /**
    * Set the mass for the rigid body
    * @param rigidbody The index for the rigid body to set
    * @param mass The mass of the rigid body
    */
    void SetMass(int rigidBodyID, float mass);

    /**
    * Sets the linear and angular damping of a rigid body
    * @param rigidbody The index for the rigid body to set
    * @param linearDamp The damping for linear velocity
    * @param angDamp The damping for angular velocity
    */
    void SetInternalDamping(int rigidBodyID, float linearDamp, float angDamp);

    /**
    * Sets whether the rigid body exists in the simulation world or not
    * @param rigidBody The index for the rigid body to set
    * @param enable Whether or not to enable the rigid body
    */
    void AddToWorld(int rigidBodyID, bool enable);

    /**
    * Resets all velocity and forces for a rigid body
    * @param rigidBody The index of the rigid body to reset
    */
    void ResetVelocityAndForce(int rigidBodyID);

    /**
    * Adds a force to a particular rigid body
    * @param force The force to add
    * @param position The position to apply the force at
    * @param rigidbody The index for the rigid body
    */
    void AddForce(const glm::vec3& force, const glm::vec3& position, int rigidBodyID);

    /**
    * Adds an impulse to a particular rigid body
    * @param force The impulse to add
    * @param position The position to apply the impulse at
    * @param rigidbody The index for the rigid body
    */
    void AddImpulse(const glm::vec3& force, const glm::vec3& position, int rigidBodyID);

    /**
    * Adds linear damping to the current amount of a rigid body
    * @param rigidBody The index for the rigid body to set
    * @param amount The damping for linear velocity
    */
    void AddLinearDamping(int rigidBodyID, float amount);

    /**
    * Adds angular damping to the current amount of a rigid body
    * @param rigidBody The index for the rigid body to set
    * @param amount The damping for angular velocity
    */
    void AddRotationalDamping(int rigidBodyID, float amount);

    /**
    * @param hinge The index for the hinge
    * @return The current rotation for the hinge
    */
    float GetHingeRotation(int hinge);

    /**
    * @param hinge The index of the hinge
    * @return whether its enabled
    */
    bool HingeEnabled(int hinge);

    /**
    * Provide rotation to the hinge constraint
    * @param hinge The index for the hinge
    * @param amount The amount of rotation to apply
    * @param dt Deltatime
    */
    void RotateHinge(int hinge, float amount, float dt);

    /**
    * Stop a hinge from rotating
    * @param hinge The index for the hinge
    * @param dt Deltatime
    * @param damping The amount of damping to apply to the stopping motion
    */
    void StopHinge(int hinge, float dt, float damping);

    /**
    * Creates a hinge constraint between two rigid bodies
    * @param rigidBody1 The first rigid body for the hinge
    * @param rigidBody2 The second rigid body for the hinge
    * @param pos1local Position local to rigidBody1 to put the constraint
    * @param pos2local Position local to rigidBody2 to put the constraint
    * @param axis1 The axis for the first rigid body
    * @param axis2 The axis for the second rigid body
    * @param breakthreshold The magnitude of force required for breakage
    */
    int CreateHinge(int rigidBodyID1, 
                    int rigidBodyID2, 
                    const glm::vec3& pos1local, 
                    const glm::vec3& pos2local, 
                    const glm::vec3& axis1, 
                    const glm::vec3& axis2, 
                    float breakthreshold = 0.0f);


    /**
    * Load a custom shape from an array of vertices
    * @param vertices The array of vertex positions
    * @return the ID of the shape
    */
    int LoadConvexShape(std::vector<glm::vec3> vertices);

    /**
    * Create a rigid body
    * @param mat The initial transform of the body
    * @param shape The collision shape to use
    * @param mass The mass of the body
    * @param group The collision group the body is part of
    * @param meshID The ID of the graphical mesh for rendering
    * @param meshInstance The instance to use of the graphical mesh
    * @param createEvents Whether the body is interested in collision events
    * @param inertia The inertia of the body
    * @return the internal index of the rigid body
    */
    int LoadRigidBody(const glm::mat4& matrix, 
                      int shape, 
                      float mass, 
                      int group, 
                      int meshID,
                      int meshInstance,
                      bool createEvents, 
                      const glm::vec3 inertia = glm::vec3());

private:

    /** 
    * Collision Masking for rigid bodies
    */
    struct CollisionFilterCallback : public btOverlapFilterCallback
    {
        /**
        * Filters objects on whether they require collision detection
        * @param proxy0/proxy1 the bodies to filter
        * @return whether the bodies require collision detection
        */
        virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, 
                                             btBroadphaseProxy* proxy1) const;
    };

    /**
    * Data for a rigid body
    */
    struct RigidBody
    {
        std::unique_ptr<btRigidBody> Body;           ///< Bullet rigid body object
        std::unique_ptr<btDefaultMotionState> State; ///< Motion state for movement interpolation
        int Mask = NO_MASK;                          ///< The collision mask of the body
        int Index = 0;                               ///< Internal index of the body in the container
        int Shape = 0;                               ///< The type of shape of the body
        int Group = 0;                               ///< The collision filter group the body belongs to
        int MeshID = 0;                              ///< ID of the graphical mesh for rendering
        int MeshInstance = 0;                        ///< Associated instance of the graphical mesh
        bool ProcessEvents = true;                   ///< Whether or not this body requires collision checking
    };

    /**
    * Prevent copying
    */
    BulletPhysicsWorld(const BulletPhysicsWorld&) = delete;
    BulletPhysicsWorld& operator=(const BulletPhysicsWorld&) = delete;

    float m_sleepvalue = 0.0f;     ///< Threshold before a body is asleep
    float m_iterations = 1.0f;     ///< Number of iterations for the world
    static const int NO_MASK = -1; ///< No collision masking

    std::vector<std::unique_ptr<btConvexHullShape>> m_shapes;            ///< Collision shapes avaliable
    std::vector<std::unique_ptr<RigidBody>> m_bodies;                    ///< Rigid bodies that exist in the scene
    std::vector<std::unique_ptr<btHingeConstraint>> m_hinges;            ///< Hinges that exist between rigid bodies
    std::unique_ptr<btDiscreteDynamicsWorld> m_world;                    ///< Bullet dynamics world
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfig;  ///< Bullet Collision configuration
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;                 ///< Bullet collision dispatcher
    std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache;       ///< Bullet collision cache
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;       ///< Bullet contraint solver
    std::unique_ptr<btOverlapFilterCallback> m_filterCallback;           ///< Custom filter callback 
};