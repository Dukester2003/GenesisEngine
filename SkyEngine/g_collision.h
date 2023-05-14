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



class ColliderShape : public GameObject
{
    public:
        Model ColliderModel;
        ColliderShape::ColliderShape()
            : GameObject() {}
        ColliderShape::ColliderShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
            : GameObject(pos, size, velocity, rotation, objModel) {}
        ColliderShape::ColliderShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation)
            : GameObject(pos, size, velocity, rotation) {}
        ColliderShape::ColliderShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model objModel)
            : GameObject(pos, size, rotation, objModel) {}
        ColliderShape::ColliderShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation)
            : GameObject(pos, size, rotation) {}
        void InitiateRigidBody(btDynamicsWorld* dynamicsWorld)
        {
             createCollisionShape();
            // Add boxShape to the collisionShapes array
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
        void UpdateRigidBody()
        {
            if (rigidBody) {
                btTransform btTrans;

                if (isDynamic) {
                    rigidBody->getMotionState()->getWorldTransform(btTrans);
                } else {
                    btTrans = rigidBody->getWorldTransform();
                }

                // Update position
                btVector3 btPos = btTrans.getOrigin();
                Position = glm::vec3(btPos.x(), btPos.y(), btPos.z());

                // Update rotation
                btQuaternion btRot = btTrans.getRotation();
                glm::quat glmRot(btRot.w(), btRot.x(), btRot.y(), btRot.z());
                Rotation = glm::eulerAngles(glmRot);
            }
        }
};

class BoxCollider : public ColliderShape
{
public:
    Model boxModel;

    int id = 0;
    static int next_id;

    void InitModel() override { boxModel = Model("colliders/box.fbx"); }
    void createCollisionShape() override {
        collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
    }

    


    BoxCollider() : ColliderShape() {}
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) { 
        type = ShapeType::BOX; 
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::BOX;
        id = next_id++;
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        auto clonedBox = std::make_shared<BoxCollider>(*this);
        clonedBox->Name = "Box " + std::to_string(next_id++);
        return clonedBox;
    }
};

class SphereCollider : public ColliderShape
{
public:
    Model sphereModel;

    int id = 0;
    static int next_id;

    void InitModel() override { sphereModel = Model("colliders/sphereCollider.obj"); }
    void createCollisionShape() override {
        btScalar radius = (Size.x / 6 + Size.y / 6 + Size.z / 6);
        collisionShape = new btSphereShape(radius);
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<SphereCollider>(*this);
    }

    SphereCollider() : ColliderShape() {}
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::SPHERE;
        id = next_id++;
    }
};

class CylinderCollider : public ColliderShape
{
public:
    Model cylinderModel;
     
    int id = 0;
    static int next_id;

    void InitModel() override { cylinderModel = Model("colliders/CylinderCollider.obj"); }
    void createCollisionShape() override {
        collisionShape = new btCylinderShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<CylinderCollider>(*this);
    }

    CylinderCollider() : ColliderShape() {}
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
};



class CapsuleCollider : public ColliderShape
{
public:
    Model capsuleModel;

    int id = 0;
    static int next_id;
    void InitModel() override { capsuleModel = Model("colliders/capsuleCollider.obj"); }
    void createCollisionShape() override {
        btScalar radius = (Size.x + Size.z) / 4;
        collisionShape = new btCapsuleShape(radius, Size.y);
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<CapsuleCollider>(*this);
    }

    CapsuleCollider() : ColliderShape() {}
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
};

class ConeCollider : public ColliderShape
{
public:
    Model coneModel;

    int id = 0;
    static int next_id;
    void InitModel() override { coneModel = Model("colliders/coneCollider.obj"); }
    void createCollisionShape() override {
        btScalar radius = (Size.x + Size.z) / 4;
        collisionShape = new btConeShape(radius, Size.y);
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<ConeCollider>(*this);
    }

    ConeCollider() : ColliderShape() {}
    ConeCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CONE;
        id = next_id++;
    }
    ConeCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::CONE;
        id = next_id++;
    }
    ConeCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::CONE;
        id = next_id++;
    }
    ConeCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::CONE;
        id = next_id++;
    }
};
class CircleFloor : public ColliderShape
{
public:
    float Radius;
    CircleFloor() : ColliderShape() {}
    CircleFloor(glm::vec3 pos, float radius, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, glm::vec3(Radius * 2, Radius * 2, Radius * 2), velocity, rotation, colliderModel), Radius(radius) {}
};

class Wall : public ColliderShape
{
    public:
        Wall() : ColliderShape() {}
        Wall(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {}
};

class Floor : public ColliderShape
{
    public:
        void createCollisionShape() override {
            collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
        }

        virtual std::shared_ptr<GameObject> clone() const override {
            return std::make_shared<Floor>(*this);
        }
        Floor() : ColliderShape() {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {}
};

class Ceiling : public ColliderShape
{
    public:
        Ceiling() : ColliderShape() {}
        Ceiling(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {}
};



class SlopeCollider : public ColliderShape
{
    public:
        float angle1 = atan2(Size.y, Size.z);
        float angle2 = atan2(Size.y, Size.x);
        float tangent1 = tan(angle1);
        float tangent2 = tan(angle2);
        float slope1 = Size.y / Size.z;
        float slope2 = Size.y / Size.x;
        Direction direction;
        void createCollisionShape() override {}

        virtual std::shared_ptr<GameObject> clone() const override {
            return std::make_shared<SlopeCollider>(*this);
        }

        SlopeCollider() : ColliderShape() {}
        SlopeCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel, Direction slopeDirection) : ColliderShape(pos, size, velocity, rotation, colliderModel) , direction(slopeDirection) {}
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
bool SATAABB(GameObject& a, ColliderShape& b);
bool SATAABB(GameObject& a, GameObject& b);
bool SATAABB(ColliderShape& a, GameObject& b);
void DoCollisions();
#endif // !COLLISION_H
