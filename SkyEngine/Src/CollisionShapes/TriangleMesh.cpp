#include "TriangleMesh.h"

int TriangleMesh::next_id = 0;

TriangleMesh::TriangleMesh() : BaseShape(), btTriangleIndexVertexArray(), Indices(std::vector<int> {}), Vertices(std::vector<btVector3> {}) {}
TriangleMesh::TriangleMesh(const std::vector<int>& indices, const std::vector<btVector3>& vertices) : btTriangleIndexVertexArray(
    indices.size() / 3,
    const_cast<int*>(indices.data()),
    sizeof(int) * 3,
    vertices.size(),
    const_cast<btScalar*>(reinterpret_cast<const btScalar*>(vertices.data())),
    sizeof(btVector3)), BaseShape()
{

    type = ShapeType::TRIANGLEMESH;
    id = next_id++;
}
TriangleMesh::~TriangleMesh() {
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

void TriangleMesh::createCollisionShape() {
    collisionShape = new btBvhTriangleMeshShape(this, true);
}

void TriangleMesh::InitiateRigidBody(btDynamicsWorld* dynamicsWorld, btAlignedObjectArray<btCollisionShape*> collisionShapes)
{
    createCollisionShape();


    btTriangleMesh* triangleMesh = new btTriangleMesh();

    for (size_t i = 0; i < Indices.size(); i += 3) {
        glm::vec3 glmVertex0 = bulletToGlm(Vertices[Indices[i]]);
        glm::vec3 glmVertex1 = bulletToGlm(Vertices[Indices[i + 1]]);
        glm::vec3 glmVertex2 = bulletToGlm(Vertices[Indices[i + 2]]);

        btVector3 vertex0(glmVertex0.x, glmVertex0.y, glmVertex0.z);
        btVector3 vertex1(glmVertex1.x, glmVertex1.y, glmVertex1.z);
        btVector3 vertex2(glmVertex2.x, glmVertex2.y, glmVertex2.z);

        triangleMesh->addTriangle(vertex0, vertex1, vertex2);
    }


    collisionShapes.push_back(collisionShape);

    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
}