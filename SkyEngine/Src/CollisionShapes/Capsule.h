#ifndef CAPSULE_SHAPE_H
#define CAPSULE_SHAPE_H
#include "BaseShape.h"

class CapsuleCollider : public BaseShape
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

    CapsuleCollider() : BaseShape() {}
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        type = ShapeType::CAPSULE;
        id = next_id++;
    }
    CapsuleCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

#endif // !CAPSULE_SHAPE_H