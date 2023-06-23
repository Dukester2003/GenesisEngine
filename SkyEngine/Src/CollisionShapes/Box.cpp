#include "Box.h"

int Box::next_id = 0;

Box::Box() : BaseShape() {}
Box::Box(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
    InitModel();
    type = ShapeType::BOX;
    id = next_id++;
}
Box::Box(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    InitModel();
    type = ShapeType::BOX;
    id = next_id++;
}
Box::Box(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    InitModel();
    type = ShapeType::BOX;
    id = next_id++;
}
Box::Box(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
    InitModel();
    type = ShapeType::BOX;
    id = next_id++;
}

Box::~Box() {
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


void Box::InitModel() { model = Model("colliders/boxCollider.obj"); }
void Box::createCollisionShape() {
    collisionShape = new btBoxShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
}

std::shared_ptr<GameObject> Box::clone() const {
    auto clonedBox = std::make_shared<Box>(*this);
    clonedBox->Name = "Box " + std::to_string(next_id++);
    return clonedBox;
}