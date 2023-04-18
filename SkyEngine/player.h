#ifndef PLAYER_H
#define PLAYER_H

#include "game_obj.h"



struct JumpActions
{
	bool jump;
	bool secondJump;
	bool superJump;
	bool jumpKick;
	bool dive;
};

struct CrouchActions
{
	bool backflip;
	bool longJump;
	bool lowKick;
	bool crawl;
};



class Player : public GameObject
{
	public:
		Player();
		Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objmodel);
		float timeAfterSpacebar;
		float timeAfterfirstJump;
		float timeAfterSecondJump;
		bool didFirstJump;
		bool didSecondJump;
		float Speed = 0.2f;
		float FirstJumpVelocity = 1.0f;
		float SecondJumpVelocity = .4f;
		float SuperJumpVelocity =  .5f;
		glm::vec3 backFlipJumpVelocity;
		float jumpSlowDown = .003f;
		int Keys[1024];
		int KeysProcessed[1024];
		JumpActions jumpActions;
		CrouchActions crouchActions;
		bool isGrounded;
		bool isMoving;
		bool isCrouched;
		bool transitionedToCrouch;
		float crouchTransTime;


	public:
		void MoveForward(float deltaTime);
		glm::quat GetForwardDirection();
		void UpdatePlayer();
		void ProcessPlayerActions(float dt);	
		void FirstJump(float dt);
		void SecondJump(float dt);
		void SuperJump(float dt);
		void Crouch();
		void SideJump();
		void BackJump(float dt);
		void LongJump();
		void GroundSlam();
		void Punch();
};

#endif // !PLAYER_H
