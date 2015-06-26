////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletPhysics.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "bulletphysics.h"
#include "CollisionEvent.h"
#include <algorithm>

namespace
{
    const float GRAVITY = -9.8f; ///< Simulated gravity
}

BulletPhysicsWorld::BulletPhysicsWorld() :
    m_collisionConfig(new btDefaultCollisionConfiguration()),
    m_dispatcher(new btCollisionDispatcher(m_collisionConfig.get())),
    m_overlappingPairCache(new btDbvtBroadphase()),
    m_solver(new btSequentialImpulseConstraintSolver()),
    m_filterCallback(new CollisionFilterCallback())
{
    m_shapes.resize(NUMBER_OF_SHAPES);

    m_world.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(),
        m_overlappingPairCache.get(), m_solver.get(), m_collisionConfig.get()));

    m_world->setGravity(btVector3(0.0f, GRAVITY, 0.0f));
    m_world->getPairCache()->setOverlapFilterCallback(m_filterCallback.get());
}

BulletPhysicsWorld::~BulletPhysicsWorld()
{
    ResetSimulation();
    m_shapes.clear();
}

void BulletPhysicsWorld::ResetSimulation()
{
    for (auto& hinge : m_hinges)
    {
        m_world->removeConstraint(hinge.get());
    }

    for (auto& rigidbody : m_bodies)
    {
        m_world->removeRigidBody(rigidbody->body.get());
    }
    
    m_hinges.clear();
    m_bodies.clear();
    m_world->clearForces();
    m_world->setGravity(btVector3(0, GRAVITY, 0));
}

bool BulletPhysicsWorld::CollisionFilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, 
                                                                          btBroadphaseProxy* proxy1) const
{
    if(proxy0->m_collisionFilterGroup == proxy1->m_collisionFilterGroup)
    {
        //groups: members of a group don't collide with each other
        return false;

    } 
    else if(proxy0->m_collisionFilterMask != BulletPhysicsWorld::NO_MASK ||
            proxy1->m_collisionFilterMask != BulletPhysicsWorld::NO_MASK)
    {
        //mask: objects with a mask only collide with the corresponding group
        return proxy0->m_collisionFilterMask == proxy1->m_collisionFilterGroup ||
               proxy1->m_collisionFilterMask == proxy0->m_collisionFilterGroup;
    }
    return true;
}

bool BulletPhysicsWorld::GenerateCollisionEvent(int collisionIndex, CollisionEvent& colEvent)
{
    btPersistentManifold* contactManifold = m_dispatcher->getManifoldByIndexInternal(collisionIndex);
    const btCollisionObject* obA = contactManifold->getBody0();
    const btCollisionObject* obB = contactManifold->getBody1();
    RigidBody* rbA = static_cast<RigidBody*>(obA->getUserPointer());
    RigidBody* rbB = static_cast<RigidBody*>(obB->getUserPointer());

    //check if any of the bodies don't want events
    if(rbA->processEvents && rbB->processEvents)
    {
        colEvent.objindex1 = rbA->userIndex;
        colEvent.objindex2 = rbB->userIndex;
        colEvent.shape1 = rbA->shape;
        colEvent.shape2 = rbB->shape;
        colEvent.body1 = rbA->index;
        colEvent.body2 = rbB->index;
        return true;        
    }
    return false;
}

void BulletPhysicsWorld::SetGroup(int rigidbody, int group)
{
    m_bodies[rigidbody]->group = group;
    btBroadphaseProxy* broadphase = m_bodies[rigidbody]->body->getBroadphaseHandle();
    if(broadphase != nullptr)
    {
        broadphase->m_collisionFilterGroup = group;
    }
}

void BulletPhysicsWorld::SetMass(int rigidbody, int shape, float mass)
{
    btVector3 localInertia(0,0,0);
    m_shapes[shape]->calculateLocalInertia(mass,localInertia);
    m_bodies[rigidbody]->body->setMassProps(mass,localInertia);
}

void BulletPhysicsWorld::ToggleEnable(int rigidbody, bool enable)
{
    enable ? AddToWorld(rigidbody, m_bodies[rigidbody]->group) : RemoveFromWorld(rigidbody);
}

int BulletPhysicsWorld::GetCollisionAmount() const
{
    return m_dispatcher->getNumManifolds();
}

void BulletPhysicsWorld::AddForce(const Float3& force, const Float3& position, int rigidbody)
{
    if(!m_bodies[rigidbody]->body->isActive())
    {
        m_bodies[rigidbody]->body->activate(true);
    }
    m_bodies[rigidbody]->body->applyForce(Float3ToBullet(force), Float3ToBullet(position));
}

float BulletPhysicsWorld::GetFriction(int rigidbody) const
{
    return m_bodies[rigidbody]->body->getFriction();
}

void BulletPhysicsWorld::AddImpulse(const Float3& force, const Float3& position, int rigidbody)
{
    if(!m_bodies[rigidbody]->body->isActive())
    {
        m_bodies[rigidbody]->body->activate(true);
    }
    m_bodies[rigidbody]->body->applyImpulse(Float3ToBullet(force),Float3ToBullet(position));
}

void BulletPhysicsWorld::SetVelocity(const Float3& velocity,
                                     int rigidbody, 
                                     float linearDamping, 
                                     float angularDamping)
{
    if(!m_bodies[rigidbody]->body->isActive())
    {
        m_bodies[rigidbody]->body->activate(true);
    }
    m_bodies[rigidbody]->body->setLinearVelocity(Float3ToBullet(velocity));
    m_bodies[rigidbody]->body->setDamping(linearDamping, angularDamping);
}

Float3 BulletPhysicsWorld::GetVelocity(int rigidbody) const
{
    return BulletToFloat3(m_bodies[rigidbody]->body->getLinearVelocity());
}

void BulletPhysicsWorld::SetInternalDamping(int rigidbody, float linearDamp, float angDamp)
{
    m_bodies[rigidbody]->body->setDamping(linearDamp, angDamp);
}

void BulletPhysicsWorld::AddLinearDamping(int rigidbody, float amount)
{
    btVector3 vec(m_bodies[rigidbody]->body->getLinearVelocity());
    vec *= amount;
    m_bodies[rigidbody]->body->setLinearVelocity(vec);
}

void BulletPhysicsWorld::AddRotationalDamping(int rigidbody, float amount)
{
    btVector3 vec(m_bodies[rigidbody]->body->getAngularVelocity());
    vec *= amount;
    m_bodies[rigidbody]->body->setAngularVelocity(vec);
}

void BulletPhysicsWorld::RemoveFromWorld(int rigidbody)
{
    m_world->removeRigidBody(m_bodies[rigidbody]->body.get());
}

void BulletPhysicsWorld::SetGravity(int rigidbody, float gravity)
{
    m_bodies[rigidbody]->body->setGravity(btVector3(0, gravity, 0));
    m_bodies[rigidbody]->body->applyGravity();
}

void BulletPhysicsWorld::SetFriction(int rigidbody, float amount)
{
    m_bodies[rigidbody]->body->setFriction(amount);
}

void BulletPhysicsWorld::AddToWorld(int rigidbody, short group, int mask)
{
    m_world->addRigidBody(m_bodies[rigidbody]->body.get(), group, mask);
}

void BulletPhysicsWorld::ResetVelocityAndForce(int rigidbody)
{
    m_bodies[rigidbody]->body->clearForces();
    m_bodies[rigidbody]->body->setAngularVelocity(btVector3(0,0,0));
    m_bodies[rigidbody]->body->setLinearVelocity(btVector3(0,0,0));
}

void BulletPhysicsWorld::SetMotionState(int rigidBody, const Matrix& matrix)
{
    btTransform& transform = m_bodies[rigidBody]->body->getWorldTransform();
    btMatrix3x3 basis;

    basis.setValue(matrix.m11, matrix.m12, matrix.m13, 
                   matrix.m21, matrix.m22, matrix.m23, 
                   matrix.m31, matrix.m32, matrix.m33);

    transform.setOrigin(Float3ToBullet(matrix.Position()));
    transform.setBasis(basis);
    m_bodies[rigidBody]->body->setWorldTransform(transform);
    m_bodies[rigidBody]->state->setWorldTransform(transform);
}

void BulletPhysicsWorld::SetBasis(int rigidBody, const Matrix& matrix)
{
    btTransform& transform = m_bodies[rigidBody]->body->getWorldTransform();
    btMatrix3x3 basis;
    basis.setValue(matrix.m11, matrix.m12, matrix.m13, 
                   matrix.m21, matrix.m22, matrix.m23, 
                   matrix.m31, matrix.m32, matrix.m33);

    transform.setBasis(basis);
    m_bodies[rigidBody]->body->setWorldTransform(transform);
    m_bodies[rigidBody]->state->setWorldTransform(transform);
}

void BulletPhysicsWorld::SetPosition(int rigidBody, const Float3& position)
{
    btTransform transform;
    m_bodies[rigidBody]->state->getWorldTransform(transform);   
    transform.setOrigin(Float3ToBullet(position));
    m_bodies[rigidBody]->body->setWorldTransform(transform);
    m_bodies[rigidBody]->state->setWorldTransform(transform);
}

Matrix BulletPhysicsWorld::GetTransform(int rigidBody) const
{
    btTransform transform;
    m_bodies[rigidBody]->state->getWorldTransform(transform);
    btMatrix3x3 basis = transform.getBasis();
        
    Matrix matrix;
    matrix.SetPosition(BulletToFloat3(transform.getOrigin()));
    matrix.SetRight(BulletToFloat3(basis.getColumn(0)));
    matrix.SetUp(BulletToFloat3(basis.getColumn(1)));
    matrix.SetForward(BulletToFloat3(basis.getColumn(2)));
    return matrix;
}

void BulletPhysicsWorld::Tick(float timestep)
{
    for (int i = 0; i < m_iterations; ++i)
    {
        m_world->stepSimulation(timestep);
    }
}

int BulletPhysicsWorld::CreateHinge(int rigidBody1, 
                                    int rigidBody2, 
                                    const Float3& pos1local, 
                                    const Float3& pos2local, 
                                    const Float3& axis1,   
                                    const Float3& axis2, 
                                    float breakthreshold)
{   
    const int index = m_hinges.size();

    m_hinges.push_back(std::unique_ptr<btHingeConstraint>(new btHingeConstraint(
                *m_bodies[rigidBody1]->body, *m_bodies[rigidBody2]->body,
                Float3ToBullet(pos1local),Float3ToBullet(pos2local),
                Float3ToBullet(axis1),Float3ToBullet(axis2))));

    m_hinges[index]->enableAngularMotor(true, 0.0f, 10.0f);

    if(breakthreshold != 0)
    {
        m_hinges[index]->setBreakingImpulseThreshold(breakthreshold);
    }

    m_world->addConstraint(m_hinges[index].get());
    return index;
}

bool BulletPhysicsWorld::HingeEnabled(int hinge)
{
    return m_hinges[hinge]->isEnabled();
}

void BulletPhysicsWorld::RotateHinge(int hinge, float amount, float dt)
{
    m_hinges[hinge]->setEnabled(true);
    btRigidBody& bodyA = m_hinges[hinge]->getRigidBodyA();
    btRigidBody& bodyB = m_hinges[hinge]->getRigidBodyB();

    if(!bodyA.isActive())
    {
       bodyA.activate(true);
    }

    if(!bodyB.isActive())
    {
        bodyB.activate(true);
    }

    m_hinges[hinge]->setMotorTarget(amount, dt);
}

float BulletPhysicsWorld::GetHingeRotation(int hinge)
{
    return m_hinges[hinge]->getHingeAngle();
}

void BulletPhysicsWorld::StopHinge(int hinge, float dt, float damping)
{
    const float velocity = m_hinges[hinge]->getMotorTargetVelosity();
    m_hinges[hinge]->setMotorTarget(m_hinges[hinge]->getHingeAngle() + (velocity * damping), dt);
}

void BulletPhysicsWorld::LoadConvexShape(float* vertices, int amount, int shape)
{
    m_shapes[shape].reset(new btConvexHullShape(vertices, amount, 3*sizeof(btScalar)));
}

void BulletPhysicsWorld::SetUserPointers()
{
    for (auto& rigidbody : m_bodies)
    {
        rigidbody->body->setUserPointer(static_cast<void*>(rigidbody.get()));
    }
}

int BulletPhysicsWorld::LoadRigidBody(const Matrix& matrix, 
                                      int shape, 
                                      float mass, 
                                      int group, 
                                      int userIndex, 
                                      bool createEvents, 
                                      int mask, 
                                      const Float3 inertia)
{
    btTransform transform;
    btMatrix3x3 basis;
    basis.setValue(matrix.m11, matrix.m12, matrix.m13, 
                   matrix.m21, matrix.m22, matrix.m23, 
                   matrix.m31, matrix.m32, matrix.m33);

    transform.setIdentity();
    transform.setOrigin(Float3ToBullet(matrix.Position()));
    transform.setBasis(basis);

    //Rigidbody is dynamic if and only if mass is non zero, otherwise static
    const bool isDynamic = mass != 0.0f;
    btVector3 localInertia(Float3ToBullet(inertia));
    if (isDynamic)
    {
        m_shapes[shape]->calculateLocalInertia(mass, localInertia);
    }

    const int index = m_bodies.size();
    m_bodies.push_back(std::unique_ptr<RigidBody>(new RigidBody()));

    //Motionstate provides interpolation capabilities, and only synchronizes 'active' objects
    m_bodies[index]->state.reset(new btDefaultMotionState(transform));

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, 
        m_bodies[index]->state.get(), m_shapes[shape].get(), localInertia);

    m_bodies[index]->body.reset(new btRigidBody(rbInfo));
    m_bodies[index]->body->setSleepingThresholds(m_sleepvalue, m_sleepvalue);
    m_bodies[index]->body->setCcdMotionThreshold(0);
    
    m_bodies[index]->shape = shape;
    m_bodies[index]->userIndex = userIndex;
    m_bodies[index]->processEvents = createEvents;
    m_bodies[index]->group = group;
    m_bodies[index]->index = index;

    m_world->addRigidBody(m_bodies[index]->body.get(), group, mask);

    return index;
}

btVector3 BulletPhysicsWorld::Float3ToBullet(const Float3& vec) const
{ 
    return btVector3(btScalar(vec.x), btScalar(vec.y), btScalar(vec.z)); 
    
}
btVector3 BulletPhysicsWorld::Float3ToBullet(float x, float y, float z) const
{
    return btVector3(btScalar(x), btScalar(y), btScalar(z)); 
}

Float3 BulletPhysicsWorld::BulletToFloat3(const btVector3 & vec) const
{ 
    return Float3(vec.getX(), vec.getY(), vec.getZ()); 
}