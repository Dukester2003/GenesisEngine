#include "Level.h"

Level::Level(Model lvlModel) {}

Level::Level() {
    InitModel();
}

Level::~Level() {}

void Level::InitModel() { model = Model("levels/warehouse/compound.glb"); }

void Level::DrawLevel(Shader shader)
{
	model.Draw(shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
    shader.setFloat("material.shininess", material.shininess);
    shader.setVec3("material.ambient", material.ambient);
    shader.setVec3("material.diffuse", material.diffuse);
    shader.setVec3("material.specular", material.specular); // specular lighting doesn't have full effect on this object's material
}

void Level::InitCollision(btDynamicsWorld* dynamicsWorld ,btAlignedObjectArray<btCollisionShape*> collisionShapes)
{

    std::vector<btVector3> bulletVertices;
   
    std::vector<int> indices;
    
    for (const auto& mesh : model.meshes) {
        indices.reserve(mesh.indices.size());
        for (const auto& vertex : mesh.vertices)
        {
            bulletVertices.push_back(btVector3(vertex.Position.x, vertex.Position.y, vertex.Position.z));
        }

        for (unsigned int index : mesh.indices) {
            indices.push_back(static_cast<int>(index));
        }

        triangleMesh = new TriangleMesh(indices, bulletVertices);
        triangleMesh->InitiateRigidBody(dynamicsWorld, collisionShapes);
    }
    
}

void Level::UpdateCollision(btDynamicsWorld* dynamicsWorld)
{
    triangleMesh->_dynamicsWorld = dynamicsWorld;
    triangleMesh->UpdateRigidBody();
}