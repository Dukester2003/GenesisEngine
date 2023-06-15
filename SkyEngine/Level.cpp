#include "Level.h"

Level::Level(Model lvlModel) {}

Level::Level() {}

void Level::DrawLevel(Model model, Shader shader)
{
	model.Draw(shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
    shader.setFloat("material.shininess", material.shininess);
    shader.setVec3("material.ambient", material.ambient);
    shader.setVec3("material.diffuse", material.diffuse);
    shader.setVec3("material.specular", material.specular); // specular lighting doesn't have full effect on this object's material
}