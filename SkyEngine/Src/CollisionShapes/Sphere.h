#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H
#include "BaseShape.h"

class SphereCollider : public BaseShape
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

    SphereCollider() : BaseShape() {}
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::SPHERE;
        id = next_id++;
    }
    SphereCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

#endif // !SPHERE_SHAPE_H