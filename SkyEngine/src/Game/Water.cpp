#include "Water.h"

Water::Water() {

}

Water::Water(glm::vec3 pos, glm::vec3 size) : GameObject(pos, size) {
	InitModel();
}