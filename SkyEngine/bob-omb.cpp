#include "bob-omb.h"

BobOmb::BobOmb()
	: Enemy() {}
BobOmb::BobOmb(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: Enemy(pos, size, velocity, rotation, objModel) {}

void BobOmb::Kaboom()
{
	this->Destroyed = true;
}