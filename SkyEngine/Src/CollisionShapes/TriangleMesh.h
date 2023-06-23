#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "BaseShape.h"

// Triangle Meshes are best only if they are static, making them dynamic could be too computationally expensive
class TriangleMesh : public BaseShape, public btTriangleIndexVertexArray
{
public:
    int id = 0;
    static int next_id;

    std::vector<int> Indices;
    std::vector<btVector3> Vertices;

    TriangleMesh();
    TriangleMesh(const std::vector<int>& indices, const std::vector<btVector3>& vertices);
    ~TriangleMesh() override;

    void createCollisionShape() override;
    void InitiateRigidBody(btDynamicsWorld* dynamicsWorld, btAlignedObjectArray<btCollisionShape*> collisionShapes) override;
};

#endif // !TRIANGLE_MESH_H
