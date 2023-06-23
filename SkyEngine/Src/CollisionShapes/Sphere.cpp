#include "Sphere.h"

int Sphere::next_id = 0;

Sphere::Sphere() : BaseShape() {}
Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
    InitModel();
    type = ShapeType::SPHERE;
    id = next_id++;
}
Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
    InitModel();
    type = ShapeType::SPHERE;
    id = next_id++;
}
Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    InitModel();
    type = ShapeType::SPHERE;
    id = next_id++;
}
Sphere::Sphere(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
    InitModel();
    type = ShapeType::SPHERE;
    id = next_id++;
}

Sphere::~Sphere() {
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

void Sphere::InitModel() { model = Model("colliders/sphereCollider.obj"); }
void Sphere::createCollisionShape()  {
    btScalar radius = (Size.x / 6 + Size.y / 6 + Size.z / 6);
    collisionShape = new btSphereShape(radius);
}

std::shared_ptr<GameObject> Sphere::clone() const {
    return std::make_shared<Sphere>(*this);
}