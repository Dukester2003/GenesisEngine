#ifndef PLAYER_H
#define PLAYER_H

#include <irrklang/irrKlang.h>
#include <random>
#include "src/Core/Object/GameObject.h"
#include "src/Scene/InputManager.h"
#include "src/Scene/Animation/Animation.h"
#include "src/Scene/Animation/Animator.h"
#include "src/Scene/Animation/AnimFlag.h"

using namespace irrklang;

struct Animations
{
	// Animation Code Here
};

class Player : public GameObject
{
	public:
		Player();
		Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
		Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		Player(btDiscreteDynamicsWorld* dynamicsWorld, const glm::vec3& position);
	private:
		glm::vec3 currentPosition;
};

#endif // !PLAYER_H
