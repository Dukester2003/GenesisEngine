#ifndef INIT_COLLISION_H
#define INIT_COLLISION_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include "scene.h"

inline btDynamicsWorld* dynamicsWorld; 
inline btBroadphaseInterface* broadphase;
inline btCollisionDispatcher* dispatcher;
inline btSequentialImpulseConstraintSolver* solver;
inline btDefaultCollisionConfiguration* collisionConfiguration;

class CollisionCallback : public btCollisionWorld::ContactResultCallback {
public:
    CollisionCallback() = default;

    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1) {
        std::cout << "Collision detected between objects " << colObj0Wrap->getCollisionObject()->getUserIndex() << " and " << colObj1Wrap->getCollisionObject()->getUserIndex() << std::endl;
        return 0; // Return 0 to ignore the contact

    }

    void BulletInstanceDispatch()
    {
        // Create the broadphase
        broadphase = new btDbvtBroadphase();

        // Set up the collision configuration and dispatcher
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);

        // Create the solver
        solver = new btSequentialImpulseConstraintSolver();

        // Create the dynamics world
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);


        dynamicsWorld->setGravity(btVector3(0, -10, 0));
        std::cout << "it works";
    }

    void UpdateCollisionCallBack(float deltaTime)
    {

        int numManifolds = dispatcher->getNumManifolds();

        std::vector<const btCollisionObject*> collidedObjects;

        for (int i = 0; i < numManifolds; ++i) {
            btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());



            // Add the colliding objects to the collidedObjects vector, if not already added
            if (std::find(collidedObjects.begin(), collidedObjects.end(), objA) == collidedObjects.end()) {
                collidedObjects.push_back(objA);
            }

            if (std::find(collidedObjects.begin(), collidedObjects.end(), objB) == collidedObjects.end()) {
                collidedObjects.push_back(objB);
            }
        }
        // Update the physics world
        dynamicsWorld->stepSimulation(deltaTime);
    }



    void UpdateBtSimulation(float deltaTime)
    {
        dynamicsWorld->stepSimulation(deltaTime, 10);

        //print positions of all objects
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
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

            std::string* objectName = static_cast<std::string*>(obj->getUserPointer());
            
        }
        
    }

    void btCleanUp()
    {
        //remove the rigidbodies from the dynamics world and delete them
        for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        //delete collision shapes
        for (int j = 0; j < collisionShapes.size(); j++)
        {
            btCollisionShape* shape = collisionShapes[j];
            collisionShapes[j] = 0;
            delete shape;
        }

        //delete dynamics world
        delete dynamicsWorld;

        //delete solver
        delete solver;

        //delete broadphase
        delete broadphase;

        //delete dispatcher
        delete dispatcher;

        delete collisionConfiguration;

        //next line is optional: it will be cleared by the destructor when the array goes out of scope
        collisionShapes.clear();
    }
};



#endif // !SURFACE_COLLISION_H


