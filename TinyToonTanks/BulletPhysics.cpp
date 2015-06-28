////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - BulletPhysics.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "BulletPhysics.h"
#include "CollisionEvent.h"
#include "Conversions.h"
#include "GameData.h"
#include <algorithm>

BulletPhysicsWorld::BulletPhysicsWorld()
{
    m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    m_overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_filterCallback = std::make_unique<CollisionFilterCallback>();
    m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());

    m_world = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(),
        m_overlappingPairCache.get(), m_solver.get(), m_collisionConfig.get());

    m_world->getPairCache()->setOverlapFilterCallback(m_filterCallback.get());

    ResetSimulation();
}

BulletPhysicsWorld::~BulletPhysicsWorld()
{
    ResetSimulation();
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
    m_world->setGravity(btVector3(0, -9.8f, 0));
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

void BulletPhysicsWorld::SetGroup(int rigidBodyID, int group)
{
    m_bodies[rigidBodyID]->group = group;
    btBroadphaseProxy* broadphase = m_bodies[rigidBodyID]->body->getBroadphaseHandle();
    if(broadphase != nullptr)
    {
        broadphase->m_collisionFilterGroup = group;
    }
}

void BulletPhysicsWorld::SetMass(int rigidBodyID, float mass)
{
    btVector3 localInertia(0,0,0);
    const int shapeID = m_bodies[rigidBodyID]->shape;
    m_shapes[shapeID]->calculateLocalInertia(mass, localInertia);
    m_bodies[rigidBodyID]->body->setMassProps(mass, localInertia);
}

void BulletPhysicsWorld::AddToWorld(int rigidBodyID, bool enable)
{
    if (enable)
    {
        m_world->addRigidBody(
            m_bodies[rigidBodyID]->body.get(), 
            m_bodies[rigidBodyID]->group,
            m_bodies[rigidBodyID]->mask);
    }
    else
    {
        m_world->removeRigidBody(m_bodies[rigidBodyID]->body.get());
    }
}

int BulletPhysicsWorld::GetCollisionAmount() const
{
    return m_dispatcher->getNumManifolds();
}

void BulletPhysicsWorld::AddForce(const glm::vec3& force, const glm::vec3& position, int rigidBodyID)
{
    if(!m_bodies[rigidBodyID]->body->isActive())
    {
        m_bodies[rigidBodyID]->body->activate(true);
    }
    m_bodies[rigidBodyID]->body->applyForce(
        Conversion::Convert(force), Conversion::Convert(position));
}

float BulletPhysicsWorld::GetFriction(int rigidBodyID) const
{
    return m_bodies[rigidBodyID]->body->getFriction();
}

void BulletPhysicsWorld::AddImpulse(const glm::vec3& force, const glm::vec3& position, int rigidBodyID)
{
    if(!m_bodies[rigidBodyID]->body->isActive())
    {
        m_bodies[rigidBodyID]->body->activate(true);
    }
    m_bodies[rigidBodyID]->body->applyImpulse(
        Conversion::Convert(force), Conversion::Convert(position));
}

void BulletPhysicsWorld::SetVelocity(const glm::vec3& velocity,
                                     int rigidBodyID, 
                                     float linearDamping, 
                                     float angularDamping)
{
    if(!m_bodies[rigidBodyID]->body->isActive())
    {
        m_bodies[rigidBodyID]->body->activate(true);
    }
    m_bodies[rigidBodyID]->body->setLinearVelocity(Conversion::Convert(velocity));
    m_bodies[rigidBodyID]->body->setDamping(linearDamping, angularDamping);
}

glm::vec3 BulletPhysicsWorld::GetVelocity(int rigidBodyID) const
{
    return Conversion::Convert(m_bodies[rigidBodyID]->body->getLinearVelocity());
}

void BulletPhysicsWorld::SetInternalDamping(int rigidBodyID, float linearDamp, float angDamp)
{
    m_bodies[rigidBodyID]->body->setDamping(linearDamp, angDamp);
}

void BulletPhysicsWorld::AddLinearDamping(int rigidBodyID, float amount)
{
    btVector3 vec(m_bodies[rigidBodyID]->body->getLinearVelocity());
    vec *= amount;
    m_bodies[rigidBodyID]->body->setLinearVelocity(vec);
}

void BulletPhysicsWorld::AddRotationalDamping(int rigidBodyID, float amount)
{
    btVector3 vec(m_bodies[rigidBodyID]->body->getAngularVelocity());
    vec *= amount;
    m_bodies[rigidBodyID]->body->setAngularVelocity(vec);
}

void BulletPhysicsWorld::SetGravity(int rigidBodyID, float gravity)
{
    m_bodies[rigidBodyID]->body->setGravity(btVector3(0, gravity, 0));
    m_bodies[rigidBodyID]->body->applyGravity();
}

void BulletPhysicsWorld::SetFriction(int rigidBodyID, float amount)
{
    m_bodies[rigidBodyID]->body->setFriction(amount);
}

void BulletPhysicsWorld::ResetVelocityAndForce(int rigidBodyID)
{
    m_bodies[rigidBodyID]->body->clearForces();
    m_bodies[rigidBodyID]->body->setAngularVelocity(btVector3(0,0,0));
    m_bodies[rigidBodyID]->body->setLinearVelocity(btVector3(0,0,0));
}

void BulletPhysicsWorld::SetMotionState(int rigidBodyID, const glm::mat4& matrix)
{
    btTransform& transform = m_bodies[rigidBodyID]->body->getWorldTransform();
    btMatrix3x3 basis(Conversion::Convert(matrix).getBasis());

    transform.setOrigin(Conversion::Convert(Conversion::Position(matrix)));
    transform.setBasis(basis);
    m_bodies[rigidBodyID]->body->setWorldTransform(transform);
    m_bodies[rigidBodyID]->state->setWorldTransform(transform);
}

void BulletPhysicsWorld::SetBasis(int rigidBodyID, const glm::mat4& matrix)
{
    btTransform& transform = m_bodies[rigidBodyID]->body->getWorldTransform();
    btMatrix3x3 basis(Conversion::Convert(matrix).getBasis());

    transform.setBasis(basis);
    m_bodies[rigidBodyID]->body->setWorldTransform(transform);
    m_bodies[rigidBodyID]->state->setWorldTransform(transform);
}

void BulletPhysicsWorld::SetPosition(int rigidBodyID, const glm::vec3& position)
{
    btTransform transform;
    m_bodies[rigidBodyID]->state->getWorldTransform(transform);   
    transform.setOrigin(Conversion::Convert(position));
    m_bodies[rigidBodyID]->body->setWorldTransform(transform);
    m_bodies[rigidBodyID]->state->setWorldTransform(transform);
}

glm::mat4 BulletPhysicsWorld::GetTransform(int rigidBodyID) const
{
    btTransform transform;
    m_bodies[rigidBodyID]->state->getWorldTransform(transform);
    return Conversion::Convert(transform);
}

void BulletPhysicsWorld::Tick(float timestep)
{
    for (int i = 0; i < m_iterations; ++i)
    {
        m_world->stepSimulation(timestep);
    }
}

int BulletPhysicsWorld::CreateHinge(int rigidBodyID1, 
                                    int rigidBodyID2, 
                                    const glm::vec3& pos1local, 
                                    const glm::vec3& pos2local, 
                                    const glm::vec3& axis1,   
                                    const glm::vec3& axis2, 
                                    float breakthreshold)
{   
    const int index = m_hinges.size();

    m_hinges.push_back(std::unique_ptr<btHingeConstraint>(new btHingeConstraint(
                *m_bodies[rigidBodyID1]->body, *m_bodies[rigidBodyID2]->body,
                Conversion::Convert(pos1local),
                Conversion::Convert(pos2local),
                Conversion::Convert(axis1),
                Conversion::Convert(axis2))));

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

int BulletPhysicsWorld::LoadConvexShape(std::vector<glm::vec3> vertices)
{
    const int ID = static_cast<int>(m_shapes.size());
    m_shapes.push_back(std::make_unique<btConvexHullShape>());

    for (const glm::vec3& vertex : vertices)
    {
        m_shapes[ID]->addPoint(Conversion::Convert(vertex));
    }

    return ID;
}

int BulletPhysicsWorld::LoadRigidBody(const glm::mat4& matrix, 
                                      int shape, 
                                      float mass, 
                                      int group, 
                                      int userIndex,
                                      bool createEvents, 
                                      int mask, 
                                      const glm::vec3 inertia)
{
    btTransform transform = Conversion::Convert(matrix);

    //Rigidbody is dynamic if and only if mass is non zero, otherwise static
    const bool isDynamic = mass != 0.0f;
    btVector3 localInertia(Conversion::Convert(inertia));
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
    m_bodies[index]->mask = mask;
    m_bodies[index]->index = index;

    m_bodies[index]->body->setUserPointer(
        static_cast<void*>(m_bodies[index].get()));

    AddToWorld(index, true);

    return index;
}