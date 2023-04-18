#include "enemy.h"

Enemy::Enemy()
	: GameObject() {}
Enemy::Enemy(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: GameObject(pos, size, velocity, rotation, objModel) {}
	