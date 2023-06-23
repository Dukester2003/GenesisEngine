#include "Cylinder.h"

int Cylinder::next_id = 0;

Cylinder::Cylinder() : BaseShape() {}
Cylinder::Cylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
    InitModel();
    type = ShapeType::CYLINDER;
    id = next_id++;
}
Cylinder::Cylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    InitModel();
    type = ShapeType::CYLINDER;
    id = next_id++;
}
Cylinder::Cylinder(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    InitModel();
    type = ShapeType::CYLINDER;
    id = next_id++;
}
Cylinder::Cylinder(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
    InitModel();
    type = ShapeType::CYLINDER;
    id = next_id++;
}

Cylinder::~Cylinder() {
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

void Cylinder::InitModel() { model = Model("colliders/CylinderCollider.obj"); }
void Cylinder::createCollisionShape() {
    collisionShape = new btCylinderShape(btVector3(btScalar(Size.x / 2), btScalar(Size.y / 2), btScalar(Size.z / 2)));
}

std::shared_ptr<GameObject> Cylinder::clone() const {
    return std::make_shared<Cylinder>(*this);
}