#ifndef GOOMBA_H
#define GOOMBA_H



#include "enemy.h"

class Goomba : public Enemy
{
	public:
		Goomba(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 rotation, Model objModel);
		Goomba();
};

#endif // !GOOMBA_H