#ifndef COMMON_ASSETS
#define COMMON_ASSETS

#include "surface_collision.h"
#include "g_collision.h"
#include "levels/terrain/Grass Block/grass_block.h"

glm::mat4 projection;
glm::mat4 view;

// Colliders
std::vector<ColliderShape> colliders;
std::vector<BoxCollider> boxColliders;
std::vector<SphereCollider> sphereColliders;
std::vector<CylinderCollider> cylinderColliders;
std::vector<CapsuleCollider> capsuleColliders;
std::vector<ConeCollider> coneColliders;
std::vector<Ceiling> ceilingColliders;
std::vector<SlopeCollider> slopeColliders;
std::vector<SlopeCollider> rSlopeColliders;
CylinderCollider circleFloorColliders[3];
Floor floorColliders[8];

// Terrain
std::vector<GrassBlock> grassBlocks;


Shader gridShader;
Shader modelShader;
Shader diffuseShader;
Shader animationShader;
Shader eulerShader;
Shader collisionShader;

// Collider Models
Model boxModel;
Model sphereModel;
Model capsuleModel;
Model coneModel;
Model floorColliderModel;
Model circleFloorColliderModel;
Model cylinderModel;
Model slopeColliderModel;
Model rSlopeColliderModel;


// Level Models
Model grassBlockModel;
Model stoneBlockModel;
void InitCommonModels()
{
    // Collider Models
    boxModel = Model("colliders/box.fbx");
    sphereModel = Model("colliders/sphereCollider.obj");
    floorColliderModel = Model("colliders/floor.obj");
    circleFloorColliderModel = Model("colliders/CircleFloor.obj");
    cylinderModel = Model("colliders/CylinderCollider.obj");
    slopeColliderModel = Model("colliders/SlopeCollider.obj");
    rSlopeColliderModel = Model("colliders/RightSlopeCollider.obj");
    capsuleModel = Model("colliders/capsuleCollider.obj");
    coneModel = Model("colliders/coneCollider.obj");

    // Level Models
    grassBlockModel = Model("levels/terrain/Grass Block/grass_block.obj");
    stoneBlockModel = Model("levels/terrain/Stone Block/stone_block.obj");
}

void InitCommonShaders()
{
    // build and compile our shaders program
    // ------------------------------------
    gridShader = Shader("shaders/grid.vs", "shaders/grid.fs");
    modelShader = Shader("shaders/model.vs", "shaders/model.fs");
    diffuseShader = Shader("shaders/diffuse.vs", "shaders/diffuse.fs");
    animationShader = Shader("shaders/anim.vs", "shaders/anim.fs");
    eulerShader = Shader("shaders/euler.vs", "shaders/euler.fs");
    collisionShader = Shader("shaders/collisionShader.vs", "shaders/collisionShader.fs");
}

void CreateShaderTransformations()
{
    // create transformations
    animationShader.setMat4("projection", projection);
    animationShader.setMat4("view", view);
    gridShader.use();
    gridShader.setMat4("projection", projection);
    gridShader.setMat4("view", view);
    collisionShader.use();
    collisionShader.setMat4("projection", projection);
    collisionShader.setMat4("view", view);
    diffuseShader.use();
    diffuseShader.setMat4("projection", projection);
    diffuseShader.setMat4("view", view);
    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
}

void UpdateCommonObjects()
{
    /// COLLIDERS
            /// ---------

    for (auto& cylinderCollider : cylinderColliders) {
        cylinderCollider._dynamicsWorld = dynamicsWorld;
        cylinderCollider.DrawModel(cylinderModel, modelShader);
        cylinderCollider.UpdateRigidBody();
    }

    for (auto& slopeCollider : slopeColliders) { slopeCollider.DrawModel(slopeColliderModel, modelShader); }
    for (auto& rSlopeCollider : rSlopeColliders) { rSlopeCollider.DrawModel(rSlopeColliderModel, modelShader); }
    for (auto& capsuleCollider : capsuleColliders) {
        if (!capsuleCollider.Destroyed)
        {
            capsuleCollider._dynamicsWorld = dynamicsWorld;
            capsuleCollider.DrawModel(capsuleModel, modelShader);
            capsuleCollider.UpdateRigidBody();
        }

    }
    for (auto& circleFloorCollider : circleFloorColliders) {
        if (!circleFloorCollider.Destroyed)
        {
            circleFloorCollider._dynamicsWorld = dynamicsWorld;
            circleFloorCollider.DrawModel(cylinderModel, modelShader);
            circleFloorCollider.UpdateRigidBody();
        }
    }
    for (int i = 0; auto& boxCollider : boxColliders) {
        boxCollider._dynamicsWorld = dynamicsWorld;
        boxCollider.DrawModel(boxModel, modelShader);
        boxCollider.UpdateRigidBody();
    }
    for (auto& sphereCollider : sphereColliders) {
            sphereCollider._dynamicsWorld = dynamicsWorld;
            sphereCollider.DrawModel(sphereModel, modelShader);
            sphereCollider.UpdateRigidBody();
    }
    for (auto& coneCollider : coneColliders) {
        if (!coneCollider.Destroyed)
        {
            coneCollider._dynamicsWorld = dynamicsWorld;
            coneCollider.DrawModel(coneModel, modelShader);
            coneCollider.UpdateRigidBody();
        }

    }
    for (auto& floorCollider : floorColliders) {
        floorCollider._dynamicsWorld = dynamicsWorld;
        floorCollider.DrawModel(boxModel, modelShader);

    }

    for (auto& grassBlock : grassBlocks) {
        grassBlock._dynamicsWorld = dynamicsWorld;
        grassBlock.DrawModel(grassBlockModel, diffuseShader);
        grassBlock.UpdateRigidBody();
    }
}
#endif // !COMMON_ASSETS


