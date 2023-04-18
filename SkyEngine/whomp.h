#include "enemy.h"

class Whomp : public Enemy
{
	public:
		Whomp(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		Whomp();

		void CrushPlayer();
};