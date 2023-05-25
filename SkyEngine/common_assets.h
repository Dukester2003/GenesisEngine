#ifndef COMMON_ASSETS
#define COMMON_ASSETS

#include "init_collision.h"
#include "g_collision.h"
#include "levels/terrain/Grass Block/blocks.h"
#include "scene.h"


glm::mat4 projection;
glm::mat4 view;

// Colliders
std::vector<ColliderShape> colliders;
std::vector<Ceiling> ceilingColliders;
std::vector<SlopeCollider> slopeColliders;
std::vector<SlopeCollider> rSlopeColliders;
CylinderCollider circleFloorColliders[3];
Floor floorColliders[8];

// Terrain
std::vector<std::shared_ptr<GrassBlock>> grassBlocks;


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
Model monkeModel;
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
    for (auto& item : items) { item->InitModel(); }
    for (auto& light : lights) { light->InitGizmo(); }
    boxModel = Model("colliders/boxCollider.obj");
    sphereModel = Model("colliders/sphereCollider.obj");
    floorColliderModel = Model("colliders/floor.obj");
    circleFloorColliderModel = Model("colliders/CircleFloor.obj");
    cylinderModel = Model("colliders/CylinderCollider.obj");
    slopeColliderModel = Model("colliders/SlopeCollider.obj");
    rSlopeColliderModel = Model("colliders/RightSlopeCollider.obj");
    capsuleModel = Model("colliders/capsuleCollider.obj");
    coneModel = Model("colliders/coneCollider.obj");
    monkeModel = Model("colliders/monke.obj");

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
    for (auto& item : items) { item->UpdateObject(item->model, diffuseShader, dynamicsWorld); }
    for (auto& slopeCollider : slopeColliders) { slopeCollider.DrawModel(slopeColliderModel, modelShader); }
    for (auto& rSlopeCollider : rSlopeColliders) { rSlopeCollider.DrawModel(rSlopeColliderModel, modelShader); }
    for (auto& circleFloorCollider : circleFloorColliders) { circleFloorCollider.UpdateObject(cylinderModel, modelShader, dynamicsWorld); }
    for (auto& floorCollider : floorColliders) { floorCollider.UpdateObject(boxModel,modelShader, dynamicsWorld); }
}

void UpdateLight()
{
    for (const auto& light : lights) {
        diffuseShader.use();
        diffuseShader.setVec3("objectColor", light->getColor());
        diffuseShader.setVec3("lightColor", light->getColor());
        diffuseShader.setVec3("lightPos", light->getPosition());

        diffuseShader.setVec3("viewPos", camera.Position);

        light->DrawGizmo(diffuseShader);
    }
}
#endif // !COMMON_ASSETS


