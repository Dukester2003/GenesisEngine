#ifndef COLLISION_H
#define COLLISION_H

#include "Math.h"
#include "GameObject.h"
#include "Scene.h"
#include "imgui/imgui.h"
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


// Objects with physics in them, probably should've came up with a better way do this.
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

// Planes will always be static, meaning they are non-moving, use only for demo purposes
class Plane : public ColliderShape
{
public:
    int id = 0;
    static int next_id;
    void createCollisionShape() override {
        collisionShape = new btStaticPlaneShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)), 10.0f);
    }
};

class BoxCollider : public ColliderShape
{
public:

    int id = 0;
    static int next_id;

    void InitModel() override { model = Model("colliders/boxCollider.obj"); }
    void createCollisionShape() override {
        collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
    }

    


    BoxCollider() : ColliderShape() {}
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) { 
        InitModel();
        type = ShapeType::BOX; 
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }

    ~BoxCollider() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
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
    int id = 0;
    static int next_id;

    void InitModel() override { model = Model("colliders/sphereCollider.obj"); }
    void createCollisionShape() override {
        btScalar radius = (Size.x / 6 + Size.y / 6 + Size.z / 6);
        collisionShape = new btSphereShape(radius);
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<SphereCollider>(*this);
    }

    SphereCollider() : ColliderShape() {}
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }

    ~SphereCollider() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};

class CylinderCollider : public ColliderShape
{
public:
    int id = 0;
    static int next_id;

    void InitModel() override { model = Model("colliders/CylinderCollider.obj"); }
    void createCollisionShape() override {
        collisionShape = new btCylinderShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<CylinderCollider>(*this);
    }

    CylinderCollider() : ColliderShape() {}
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }

    ~CylinderCollider() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};



class CapsuleCollider : public ColliderShape
{
public:
    int id = 0;
    static int next_id;
    void InitModel() override { model = Model("colliders/capsuleCollider.obj"); }
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
        InitModel();
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
    ~CapsuleCollider() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};

class ConeCollider : public ColliderShape
{
public:
    int id = 0;
    static int next_id;
    void InitModel() override { model = Model("colliders/coneCollider.obj"); }
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
        InitModel();
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
    ~ConeCollider() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};

// Since the compound shape is going to have multiple objects, multiple models, the models are not going to be static
class CompoundShape : public ColliderShape
{
public:
    Model monkeModel;

    int id = 0;
    static int next_id;
    void InitModel() override { monkeModel = Model("colliders/monke.obj"); }
    void createCollisionShape() override {
        btCompoundShape* compound = new btCompoundShape();

        for (ColliderShape* childShape : childShapes) {
            childShape->createCollisionShape();
            btTransform childTransform; // set this as necessary
            compound->addChildShape(childTransform, childShape->getBtCollisionShape());
        }

        // Store 'compound' somewhere safe where it can be deleted later
        setBtCollisionShape(compound);
    }

    // Add a child shape to the compound shape
    void AddChildShape(ColliderShape* shape) {
        childShapes.push_back(shape);
    }

    // Remove a child shape from the compound shape
    void RemoveChildShape(ColliderShape* shape) {
        //... implement this
    }

    virtual std::shared_ptr<GameObject> clone() const override {
        return std::make_shared<CompoundShape>(*this);
    }

    void ObjMenu(string name) override;
        
    
    CompoundShape() : ColliderShape() {}
    CompoundShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::COMPOUND;
        id = next_id++;
    }
    CompoundShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::COMPOUND;
        id = next_id++;
    }

private:
    std::vector<ColliderShape*> childShapes;
};

class ConvexHull : public ColliderShape
{
public:

    int id = 0;
    static int next_id;
    ConvexHull() : ColliderShape() {}
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ~ConvexHull() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
    
};

class TriangleMesh : public ColliderShape
{
public:

    int id = 0;
    static int next_id;
    TriangleMesh() : ColliderShape() {}
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    ~TriangleMesh() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }

};

class HeightField : public ColliderShape
{
public:
    int id = 0;
    static int next_id;
    HeightField() : ColliderShape() {}
    HeightField(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    ~HeightField() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};

class SoftBody : public ColliderShape
{
public:
    int id = 0;
    static int next_id;
    SoftBody() : ColliderShape() {}
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : ColliderShape(pos, size, rotation) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, rotation, colliderModel) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    ~SoftBody() override {
        if (rigidBody) {
            if (rigidBody->getMotionState()) {
                delete rigidBody->getMotionState();
            }

            // Assuming you have access to the dynamicsWorld instance here
            _dynamicsWorld->removeRigidBody(rigidBody);
            delete rigidBody;
        }

        // Delete the shape if it's not shared with other bodies
        if (collisionShape) {
            delete collisionShape;
        }
    }
};

class MultiSphere : public ColliderShape
{
public:

};

class ConvexPointCloud : public ColliderShape
{
public:

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

        void InitModel() override { model = Model("colliders/boxCollider.obj"); }

        virtual std::shared_ptr<GameObject> clone() const override {
            return std::make_shared<Floor>(*this);
        }
        Floor() : ColliderShape() {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : ColliderShape(pos, size, velocity, rotation, colliderModel) {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : ColliderShape(pos, size, velocity, rotation) 
        {
            InitModel();
            type = ShapeType::BOX;
        }
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
