#include "goomba.h"

Goomba::Goomba()
	: Enemy() {}
Goomba::Goomba(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: Enemy(pos, size, velocity, rotation, objModel) {}