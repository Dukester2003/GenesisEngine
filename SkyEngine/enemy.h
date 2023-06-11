#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject 
{
	public:
		Enemy(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		Enemy();
};

#endif // !ENEMY_H
