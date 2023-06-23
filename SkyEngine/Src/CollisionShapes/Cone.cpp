#include "Cone.h"

int Cone::next_id = 0;

Cone::Cone() : BaseShape() {}
Cone::Cone(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
    type = ShapeType::CONE;
    id = next_id++;
}
Cone::Cone(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    InitModel();
    type = ShapeType::CONE;
    id = next_id++;
}
Cone::Cone(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    type = ShapeType::CONE;
    id = next_id++;
}
Cone::Cone(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
    type = ShapeType::CONE;
    id = next_id++;
}
Cone::~Cone() {
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

void Cone::InitModel() { model = Model("colliders/coneCollider.obj"); }
void Cone::createCollisionShape() {
    btScalar radius = (Size.x + Size.z) / 4;
    collisionShape = new btConeShape(radius, Size.y);
}

std::shared_ptr<GameObject> Cone::clone() const {
    return std::make_shared<Cone>(*this);
}