#ifndef BOB_OMB_H
#define BOB_OMB_H

#include "enemy.h"



class BobOmb : public Enemy
{
	public:
		BobOmb(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		BobOmb();
		void Kaboom();

		
};
#endif // !BOB_OMB_H
