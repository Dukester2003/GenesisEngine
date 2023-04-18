#ifndef COLLISION_H
#define COLLISION_H

#include "math.h"
#include "game_obj.h"
#include "scene.h"
using namespace glm;

enum Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    UP,
    DOWN,
};

class Collider : public GameObject
{
    public:
        Collider::Collider()
            : GameObject() {}
        Collider::Collider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
            : GameObject(pos, size, velocity, rotation, objModel) {}

};
class Wall : public Collider
{
    public:
        Wall() : Collider() {}
        Wall(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, size, velocity, rotation, colliderModel) {}
};

class Floor : public Collider
{
    public:
        Floor() : Collider() {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, size, velocity, rotation, colliderModel) {}
};

class Ceiling : public Collider
{
    public:
        Ceiling() : Collider() {}
        Ceiling(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, size, velocity, rotation, colliderModel) {}
};

class BoxCollider : public Collider
{
    public:
        btCollisionShape* boxShape = new btBoxShape(btVector3(btScalar(Size.x/2), btScalar(Size.y/2), btScalar(Size.z/2)));
        void InitiateRigidBody(btDiscreteDynamicsWorld* dynamicsWorld)
        {
            // Add boxShape to the collisionShapes array
            collisionShapes.push_back(boxShape);


            btTransform startTransform;
            startTransform.setIdentity();
            startTransform.setOrigin(glmToBullet(Position));


            btScalar mass(5.);

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(1.0, 1.0, 1.0);
            if (isDynamic)
                boxShape->calculateLocalInertia(mass, localInertia);

            //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
            rigidBody = new btRigidBody(rbInfo);
            rigidBody->setFriction(.8f);

            //add the body to the dynamics world
            dynamicsWorld->addRigidBody(rigidBody);
        }
        void UpdateRigidBody()
        {
            if (rigidBody) {
                btTransform btTrans;
                rigidBody->getMotionState()->getWorldTransform(btTrans);

                // Update position
                btVector3 btPos = btTrans.getOrigin();
                Position = glm::vec3(btPos.x(), btPos.y(), btPos.z());

                // Update rotation
                btQuaternion btRot = btTrans.getRotation();
                glm::quat glmRot(btRot.w(), btRot.x(), btRot.y(), btRot.z());
                Rotation = glm::eulerAngles(glmRot);
            }
        }
        BoxCollider() : Collider() {}
        BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, size, velocity, rotation, colliderModel) {}
};

class SphereCollider : public Collider
{
    public:
        float Radius;
        SphereCollider() : Collider() {}
        SphereCollider(glm::vec3 pos, float radius, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, glm::vec3(Radius * 2, Radius * 2, Radius * 2), velocity, rotation, colliderModel), Radius(radius) {}
};
class CylinderCollider : public Collider
{
    public:
        btCollisionShape* cylinderShape = new btCylinderShape(btVector3(btScalar(Size.x), btScalar(Size.y), btScalar(Size.z)));
        void InitiateRigidBody(btDiscreteDynamicsWorld* dynamicsWorld)
        {
            // Add boxShape to the collisionShapes array
            collisionShapes.push_back(cylinderShape);


            btTransform startTransform;
            startTransform.setIdentity();
            startTransform.setOrigin(glmToBullet(Position));


            btScalar mass(0.);

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0, 0, 0);
            if (isDynamic)
                cylinderShape->calculateLocalInertia(mass, localInertia);

            //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cylinderShape, localInertia);
            rigidBody = new btRigidBody(rbInfo);


            //add the body to the dynamics world
            dynamicsWorld->addRigidBody(rigidBody);
        }
        void UpdateRigidBody()
        {
            if (rigidBody) {
                btTransform btTrans;
                rigidBody->getMotionState()->getWorldTransform(btTrans);

                // Update position
                btVector3 btPos = btTrans.getOrigin();
                Position = glm::vec3(btPos.x(), btPos.y(), btPos.z());

                // Update rotation
                btQuaternion btRot = btTrans.getRotation();
                glm::quat glmRot(btRot.w(), btRot.x(), btRot.y(), btRot.z());
                Rotation = glm::eulerAngles(glmRot);
            }
        }
        CylinderCollider() : Collider() {}
        CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, size, velocity, rotation, colliderModel) {}
};
class CircleFloor : public Collider
{
    public:
        float Radius;
        CircleFloor() : Collider() {}
        CircleFloor(glm::vec3 pos, float radius, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : Collider(pos, glm::vec3(Radius * 2, Radius * 2, Radius * 2), velocity, rotation, colliderModel), Radius(radius) {}
};

class SlopeCollider : public Collider
{
    public:
        float angle1 = atan2(Size.y, Size.z);
        float angle2 = atan2(Size.y, Size.x);
        float tangent1 = tan(angle1);
        float tangent2 = tan(angle2);
        float slope1 = Size.y / Size.z;
        float slope2 = Size.y / Size.x;
        Direction direction;
        SlopeCollider() : Collider() {}
        SlopeCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel, Direction slopeDirection) : Collider(pos, size, velocity, rotation, colliderModel) , direction(slopeDirection) {}
};

bool AABB(GameObject& a, GameObject& b);
bool SphereCollide(GameObject& gameObject, SphereCollider& collider);
bool CylinderCollide(GameObject& gameObject, CylinderCollider& collider);
bool CFloorCollide(GameObject& gameObject, CircleFloor& collider);
bool SlopeCollide(GameObject& gameObject, SlopeCollider& collider);
// compute the magnitude of a vector
float magnitude(const vec3& v);
vec3 normal(const vec3& a, const vec3& b);
vec3 normal(const Vertex& a, const Vertex& b);
vec3 normal(const vec3& a, const Vertex& b);
float vertDot(const vec3& a, const Vertex& b);
void processMeshCollider(aiMesh* mesh, const aiScene* scene);
bool SAT(GameObject& a, GameObject& b);
bool SATAABB(GameObject& a, Collider& b);
bool SATAABB(GameObject& a, GameObject& b);
bool SATAABB(Collider& a, GameObject& b);
void DoCollisions();
#endif // !COLLISION_H
