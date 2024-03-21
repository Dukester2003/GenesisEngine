#include "BaseShape.h"



int ConvexHull::next_id = 0;
int HeightField::next_id = 0;
int SoftBody::next_id = 0;
/*
int MultiSphere::next_id = 0;
int ConvexPointCloud::next_id = 0;
*/

void BaseShape::InitiateRigidBody(btDynamicsWorld* dynamicsWorld, btAlignedObjectArray<btCollisionShape*> collisionShapes)
{
    createCollisionShape();
    // Add Shape to the collisionShapes array
    collisionShapes.push_back(collisionShape);


    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(glmToBullet(Position));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    isDynamic = (massValue != 0.f);

    localInertia = btVector3(1.0, 1.0, 1.0);
    collisionShape->calculateLocalInertia(massValue, localInertia);

    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(massValue, myMotionState, collisionShape, localInertia);
    rigidBody = new btRigidBody(rbInfo);
    rigidBody->setFriction(.8f);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(rigidBody);
}

void BaseShape::UpdateRigidBody()
{
    if (rigidBody) {
        btTransform btTrans;

        if (isDynamic) {
            rigidBody->getMotionState()->getWorldTransform(btTrans);
        }
        else {
            btTrans = rigidBody->getWorldTransform();
        }

        // Update position
        btVector3 btPos = btTrans.getOrigin();
        Position = glm::vec3(btPos.x(), btPos.y(), btPos.z());

        // Update rotation
        btQuaternion btRot = btTrans.getRotation();
        glm::quat glmRot(btRot.w(), btRot.x(), btRot.y(), btRot.z());
        Rotation = glmRot;

        std::cout << "X Rotation = " << Rotation.x << endl << "Y Rotation = " << Rotation.y << endl << "Z Rotation = " << Rotation.z << endl;
    }
}


