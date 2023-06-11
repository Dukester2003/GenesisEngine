#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "g_animation.h"
#include "g_animator.h"
#include "AnimFlag.h"


struct Animations
{
	// THE PLAYER SETTINGS
	// ----------
	Model playerIdleModel;
	Animation idleAnimation;
	Animation runningAnimation;
	Animation firstJumpAnimation;
	Animation secondJumpAnimation;
	Animation superJumpAnimation;
	Animation crouchTransition;
	Animation crouchAnimation;
	Animation backJumpAnimation;
	Animation longJumpAnimation;
};

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
		Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
		Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		Player(btDiscreteDynamicsWorld* dynamicsWorld, const glm::vec3& position);
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
		Animator animator;
		Animations animations;
		AnimationFlag animFlag;
		AnimationID animID;
		CrouchActions crouchActions;
		bool isGrounded;
		bool isMoving;
		bool isCrouched;
		bool transitionedToCrouch;
		float crouchTransTime;


	public:	
		void createCollisionShape() override { /* Do nothing */ };
		std::shared_ptr<GameObject> clone() const override { return NULL; };
		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);
		glm::quat GetForwardDirection();
		void UpdatePlayer(float deltaTime);
		void BoneTransforms(Shader shader);
		void UpdateAnimations();
		void ProcessPlayerActions(float dt);	
		void InitAnimations();
		void FirstJump(float dt);
		void SecondJump(float dt);
		void SuperJump(float dt);
		void Crouch();
		void SideJump();
		void BackJump(float dt);
		void LongJump();
		void GroundSlam();
		void Punch();
	private:
		glm::vec3 currentPosition;
};

#endif // !PLAYER_H
