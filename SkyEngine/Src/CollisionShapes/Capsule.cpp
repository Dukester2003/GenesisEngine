#include "Capsule.h"

int Capsule::next_id = 0;

Capsule::Capsule() : BaseShape() {}
Capsule::Capsule(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
    type = ShapeType::CAPSULE;
    id = next_id++;
}
Capsule::Capsule(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    InitModel();
    type = ShapeType::CAPSULE;
    id = next_id++;
}
Capsule::Capsule(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    type = ShapeType::CAPSULE;
    id = next_id++;
}
Capsule::Capsule(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
    type = ShapeType::CAPSULE;
    id = next_id++;
}
Capsule::~Capsule() {
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


void Capsule::InitModel() { model = Model("colliders/capsuleCollider.obj"); }
void Capsule::createCollisionShape() {
    btScalar radius = (Size.x + Size.z) / 4;
    collisionShape = new btCapsuleShape(radius, Size.y);
}

std::shared_ptr<GameObject> Capsule::clone() const {
    return std::make_shared<Capsule>(*this);
}