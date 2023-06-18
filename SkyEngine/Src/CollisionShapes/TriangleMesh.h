#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "BaseShape.h"

// Triangle Meshes are best only if they are static, making them dynamic could be too computationally expensive
class TriangleMesh : public BaseShape
{
public:

    int id = 0;
    static int next_id;
    TriangleMesh() : BaseShape() {}
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, velocity, rotation, colliderModel) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation) : BaseShape(pos, size, velocity, rotation) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::quat rotation) : BaseShape(pos, size, rotation) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    TriangleMesh(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model colliderModel) : BaseShape(pos, size, rotation, colliderModel) {
        type = ShapeType::TRIANGLEMESH;
        id = next_id++;
    }
    ~TriangleMesh() override {
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

#endif // !TRIANGLE_MESH_H
