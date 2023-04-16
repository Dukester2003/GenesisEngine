#ifndef ENEMY_H
#define ENEMY_H

#include "game_obj.h"

class Enemy : public GameObject 
{
	public:
		Enemy(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 rotation, Model objmodel);
		Enemy();
};

#endif // !ENEMY_H
