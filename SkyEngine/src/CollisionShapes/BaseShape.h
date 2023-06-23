#ifndef COLLISION_H
#define COLLISION_H

#include "Math.h"
#include "../Core/Object/GameObject.h"
#include "../../imgui/imgui.h"

using namespace glm;

// Objects with physics in them, probably should've came up with a better way do this.
class BaseShape : public GameObject
{
    public:
        Model ColliderModel;
        BaseShape::BaseShape()
            : GameObject() {}
        BaseShape::BaseShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
            : GameObject(pos, size, velocity, rotation, objModel) {}
        BaseShape::BaseShape(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation)
            : GameObject(pos, size, velocity, rotation) {}
        BaseShape::BaseShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model objModel)
            : GameObject(pos, size, rotation, objModel) {}
        BaseShape::BaseShape(glm::vec3 pos, glm::vec3 size, glm::quat rotation)
            : GameObject(pos, size, rotation) {}

        void InitiateRigidBody(btDynamicsWorld* dynamicsWorld, btAlignedObjectArray<btCollisionShape*> collisionShapes);
        void UpdateRigidBody();
};

class ConvexHull : public BaseShape
{
public:

    int id = 0;
    static int next_id;
    ConvexHull() : BaseShape() {}
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        type = ShapeType::CONVEXHULL;
        id = next_id++;
    }
    ConvexHull(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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



class HeightField : public BaseShape
{
public:
    int id = 0;
    static int next_id;
    HeightField() : BaseShape() {}
    HeightField(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        type = ShapeType::HEIGHTFIELD;
        id = next_id++;
    }
    HeightField(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

class SoftBody : public BaseShape
{
public:
    int id = 0;
    static int next_id;
    SoftBody() : BaseShape() {}
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        type = ShapeType::SOFTBODY;
        id = next_id++;
    }
    SoftBody(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

class MultiSphere : public BaseShape
{
public:

};

class ConvexPointCloud : public BaseShape
{
public:

};

class Floor : public BaseShape
{
    public:
        void createCollisionShape() override {
            collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
        }

        void InitModel() override { model = Model("colliders/boxCollider.obj"); }

        virtual std::shared_ptr<GameObject> clone() const override {
            return std::make_shared<Floor>(*this);
        }
        Floor() : BaseShape() {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {}
        Floor(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) 
        {
            InitModel();
            type = ShapeType::BOX;
        }
};

#endif // !COLLISION_H
