#pragma once

#include "game_obj.h"

class KingBobOmb : public GameObject
{
	public:
		KingBobOmb(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 rotation, Model objModel);
		KingBobOmb();

		void PlayCutscene();
};