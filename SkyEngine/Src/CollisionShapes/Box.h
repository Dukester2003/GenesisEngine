#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H
#include "BaseShape.h"

class BoxCollider : public BaseShape
{
public:

    int id = 0;
    static int next_id;

    void InitModel() override { model = Model("colliders/boxCollider.obj"); }
    void createCollisionShape() override {
        collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
    }



    BoxCollider() : BaseShape() {}
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        InitModel();
        type = ShapeType::BOX;
        id = next_id++;
    }
    BoxCollider(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
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

#endif // !BOX_SHAPE_H




