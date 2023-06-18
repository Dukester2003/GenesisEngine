#ifndef CYLINDER_H
#define CYLINDER_H

#include "BaseShape.h"

class CylinderCollider : public BaseShape
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

    CylinderCollider() : BaseShape() {}
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::CYLINDER;
        id = next_id++;
    }
    CylinderCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

#endif // !CYLINDER_H

