#ifndef COMMON_ASSETS
#define COMMON_ASSETS

#include "InitiateCollision.h"
#include "BaseShape.h"
#include "levels/terrain/Grass Block/blocks.h"
#include "Scene.h"


inline glm::mat4 projection;
inline glm::mat4 view;

// Colliders
inline Floor floorColliders[8];




inline Shader gridShader;
inline Shader modelShader;
inline Shader diffuseShader;
inline Shader animationShader;
inline Shader eulerShader;
inline Shader collisionShader;



inline void InitCommonShaders()
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

inline void CreateShaderTransformations()
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

inline void UpdateCommonObjects(Scene& scene)
{
    for (auto& item : scene.items) { item->UpdateObject(diffuseShader, dynamicsWorld); }
    for (auto& floorCollider : floorColliders) { floorCollider.UpdateObject(modelShader, dynamicsWorld); }
}

inline void InitMaterial(Scene& scene)
{
    for (auto& item : scene.items) {
        diffuseShader.use();
        diffuseShader.setFloat("material.shininess", item->material.shininess);
        diffuseShader.setVec3("material.ambient", item->material.ambient);
        diffuseShader.setVec3("material.diffuse", item->material.diffuse);
        diffuseShader.setVec3("material.specular", item->material.specular);
    }
}
inline void UpdateLight(Scene& scene)
{
    diffuseShader.use();
    scene.UpdateDirLights(diffuseShader);
    scene.UpdatePointLights(diffuseShader);
    scene.UpdateSpotLights(diffuseShader);

    diffuseShader.setVec3("viewPos", scene.camera.Position);

    for(auto& item: scene.items) {
        diffuseShader.use();      
        diffuseShader.setFloat("material.shininess", item->material.shininess);

    }
}
#endif // !COMMON_ASSETS


