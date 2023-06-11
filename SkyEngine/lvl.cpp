#include "Level.h"

Level::Level(Model lvlModel) {}

Level::Level() {}

void Level::DrawLevel(Model model, Shader shader)
{
	model.Draw(shader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
}