#ifndef COMMON_ASSETS
#define COMMON_ASSETS

#include "game_obj.h"
#include "g_collision.h"

glm::mat4 projection;
glm::mat4 view;

// Colliders
std::vector<Collider> colliders;
std::vector<BoxCollider> boxColliders;
std::vector<Ceiling> ceilingColliders;
std::vector<SlopeCollider> slopeColliders;
std::vector<SlopeCollider> rSlopeColliders;
std::vector<CylinderCollider> cylinderColliders;

Shader gridShader;
Shader modelShader;
Shader animationShader;
Shader eulerShader;
Shader collisionShader;

// Collider Models
Model boxModel;
Model floorColliderModel;
Model circleFloorColliderModel;
Model cylinderModel;
Model slopeColliderModel;
Model rSlopeColliderModel;

void InitCommonModels()
{
    // Collider Models
    boxModel = Model("colliders/box.fbx");
    floorColliderModel = Model("colliders/floor.obj");
    circleFloorColliderModel = Model("colliders/CircleFloor.obj");
    cylinderModel = Model("colliders/CylinderCollider.obj");
    slopeColliderModel = Model("colliders/SlopeCollider.obj");
    rSlopeColliderModel = Model("colliders/RightSlopeCollider.obj");
}

void InitCommonShaders()
{
    // build and compile our shaders program
    // ------------------------------------
    gridShader = Shader("shaders/grid.vs", "shaders/grid.fs");
    modelShader = Shader("shaders/model.vs", "shaders/model.fs");
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
    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
}
#endif // !COMMON_ASSETS


