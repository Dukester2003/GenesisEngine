#ifndef WATER_H
#define WATER_H

#include "../Core/Object/GameObject.h"

class Water : public GameObject
{
public:
	Water();
	Water(glm::vec3 pos, glm::vec3 size);

	virtual void InitModel() override {
		model = Model("levels/warehouse/water.glb");
	}
};
#endif // !WATER_H
