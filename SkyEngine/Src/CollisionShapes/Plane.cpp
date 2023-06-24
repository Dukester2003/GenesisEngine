#include "Plane.h"

int Plane::next_id = 0;

Plane::Plane() : BaseShape() {}

Plane::Plane(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
    InitModel();
    type = ShapeType::PLANE;
    id = next_id++;
}

void Plane::InitModel() { model = Model("colliders/floor.obj"); }