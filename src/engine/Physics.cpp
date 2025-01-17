#include "physics/Physics.hpp"

namespace lei3d
{
    PhysicsObjects CreatePhysicsWorld()
    {
        // this is just the default setup, we can make this fancier if we want
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        // good general purpose broadphase, we cna also use btAxis3Sweep
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -1, 0));

        // Create collisionShapes object to keep track of all the objects we want to collide over (this will eventually include the whole scene)
        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        // Create a character capsule that obeys the laws of gravity.
        btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
        collisionShapes.push_back(character);

        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass{1.f};

        btVector3 localInertia{0.0f, 0.0f, 0.0f};
        character->calculateLocalInertia(mass, localInertia);
        startTransform.setOrigin(btVector3{3.0f, 50.0f, 0.0f});
        
        btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
        btRigidBody* characterBody = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(characterBody);

        // Now make the ground 
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        collisionShapes.push_back(groundShape);
        
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -70, 0));

        btScalar floorMass = 0.0f;
        btVector3 floorLocalInertia{0.0f, 0.0f, 0.0f}; // list initialization makes my brain happy with structs. I'm sorry for the sudden switch haha
        
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbFloorInfo{floorMass, myMotionState, groundShape, floorLocalInertia};
        btRigidBody* floorBody = new btRigidBody(rbFloorInfo);

        dynamicsWorld->addRigidBody(floorBody);
        
        // return our physics objects, will be modified before passed to physicsStep
        PhysicsObjects objects {
            collisionConfiguration, 
            dispatcher, 
            overlappingPairCache, 
            solver, 
            dynamicsWorld,           
            collisionShapes
        };

        return objects;
    }

    void PhysicsStep(PhysicsObjects physicsObjects, float deltaTime)
    {
        physicsObjects.dynamicsWorld->stepSimulation(deltaTime, 10);

        // Move every object
		for (int j = physicsObjects.dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = physicsObjects.dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
		}
    }

    glm::vec3 GetFirstColliderPosition(PhysicsObjects physicsObjects)
    {
        btCollisionObject* obj = physicsObjects.dynamicsWorld->getCollisionObjectArray()[0];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }
        glm::vec3 position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
        return position;
    }


}