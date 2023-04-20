#include "player.h"
#include <GLFW/glfw3.h>



Player::Player()
	: GameObject() {}
Player::Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: GameObject(pos, size, velocity, rotation, objModel), Keys(), KeysProcessed() {}


void Player::MoveBackward(float deltaTime) {
	glm::quat backwardDirection = -GetForwardDirection();
	Position += backwardDirection * Velocity;
}
void Player::MoveForward(float deltaTime) {
	glm::quat forwardDirection = GetForwardDirection();
	Position += forwardDirection * Velocity;
}

glm::quat Player::GetForwardDirection() {
	return Rotation;
}

void Player::UpdatePlayer()
{
	backFlipJumpVelocity = glm::vec3(-.2f * sin(Rotation.y * 3.14159 / 180), 0.8, -.2f * cos(Rotation.y * 3.14159 / 180));
	Velocity = glm::vec3(Speed * sin(Rotation.y * glm::pi<float>() / 180), 0.0, Speed * cos(Rotation.y * glm::pi<float>() / 180));
}
void Player::FirstJump(float dt)
{
	Velocity.y = FirstJumpVelocity;
	timeAfterSpacebar += dt;
	timeAfterfirstJump += dt;
	if (timeAfterSpacebar < .15f)
	{	
		Position.y += Velocity.y;
	}
}
void Player::SecondJump(float dt)
{
	Velocity.y = SecondJumpVelocity;
	timeAfterSpacebar += dt;
	timeAfterSecondJump += dt;
	if (timeAfterSpacebar < .15f)
	{
		Position.y += Velocity.y;
	}
}

void Player::SuperJump(float dt)
{
	Velocity.y = SuperJumpVelocity;
	timeAfterSpacebar += dt;	
	if (timeAfterSpacebar < .15f)
	{
		Position.y += Velocity.y;
	}
}

void Player::BackJump(float dt)
{
	Velocity = backFlipJumpVelocity;
	timeAfterSpacebar += dt;
	if (timeAfterSpacebar < .15f)
	{
		MoveBackward(dt);
	}
}