#include "player.h"
#include <GLFW/glfw3.h>



Player::Player()
	: GameObject() {}
Player::Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: GameObject(pos, size, velocity, rotation, objModel), Keys(), KeysProcessed() {}
Player::Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation)
	: GameObject(pos, size, velocity, rotation), Keys(), KeysProcessed(), animations() {}

Player::Player(btDiscreteDynamicsWorld* dynamicsWorld, const glm::vec3& position) {
	this->_dynamicsWorld = dynamicsWorld;
	currentPosition = position;

	btCollisionShape* capsuleShape = new btCapsuleShape(0.5f, 1.8f); // Adjust the size to fit your player model

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	float mass = 80.0f; // Set the mass for the player
	btVector3 localInertia(0, 0, 0);
	capsuleShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, capsuleShape, localInertia);

	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setAngularFactor(btVector3(0, 0, 0)); // Prevent the player from rotating
	rigidBody->setFriction(1.0f);
	rigidBody->setRestitution(0.0f);
	rigidBody->setActivationState(DISABLE_DEACTIVATION); // Keep the player active at all times

	dynamicsWorld->addRigidBody(rigidBody);
}


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

void Player::InitAnimations()
{
	// THE PLAYER SETTINGS
	// ----------
	animations.playerIdleModel = Model("player/luigiIdle.dae");
	animations.idleAnimation = Animation("player/luigiIdle.dae", &animations.playerIdleModel);
	animations.runningAnimation = Animation("player/luigiRunning.dae", &animations.playerIdleModel);
	animations.firstJumpAnimation = Animation("player/luigiFirstJump.dae", &animations.playerIdleModel);
	animations.secondJumpAnimation = Animation("player/luigiSecondJump.dae", &animations.playerIdleModel);
	animations.superJumpAnimation = Animation("player/luigiSuperJump.dae", &animations.playerIdleModel);
	animations.crouchTransition = Animation("player/luigiCrouchTransition.dae", &animations.playerIdleModel);
	animations.crouchAnimation = Animation("player/luigiCrouch.dae", &animations.playerIdleModel);
	animations.backJumpAnimation = Animation("player/luigiBackJump.dae", &animations.playerIdleModel);
	animations.longJumpAnimation = Animation("player/luigiLongJump.dae", &animations.playerIdleModel);
}

void Player::BoneTransforms(Shader shader)
{
	shader.use();

	auto transforms = animator.GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

	DrawModel(animations.playerIdleModel, shader);
}

void Player::UpdatePlayer(float deltaTime)
{
	backFlipJumpVelocity = glm::vec3(-.2f * sin(Rotation.y * 3.14159 / 180), 0.8, -.2f * cos(Rotation.y * 3.14159 / 180));
	Velocity = glm::vec3(Speed * sin(Rotation.y * glm::pi<float>() / 180), 0.0, Speed * cos(Rotation.y * glm::pi<float>() / 180));
	UpdateAnimations();
}

void Player::UpdateAnimations()
{
	struct AnimationDetails {
		bool* flag;
		Animation* animation;
	};

	std::map<int, AnimationDetails> animationMap
	{
		{ PLAYER_IDLE_ID, {&animFlag.playerIdle, &animations.idleAnimation} },
		{ PLAYER_RUN_ID, {&animFlag.playerRun, &animations.runningAnimation} },
		{ PLAYER_JUMP_ID, {&animFlag.playerJump, &animations.firstJumpAnimation} },
		{ PLAYER_SECOND_JUMP_ID, {&animFlag.playerSecondJump, &animations.secondJumpAnimation} },
		{ PLAYER_SUPER_JUMP_ID, {&animFlag.playerSuperJump, &animations.superJumpAnimation} },
		{ PLAYER_CROUCH_TRANSITION_ID, {&animFlag.playerCrouchTransition, &animations.crouchTransition} },
		{ PLAYER_CROUCH_ID, {&animFlag.playerCrouch, &animations.crouchAnimation} },
		{ PLAYER_BACK_JUMP_ID, {&animFlag.playerBackJump, &animations.backJumpAnimation} }
	};

	for (auto& anim : animationMap) {
		if (animID == anim.first && !(*anim.second.flag)) {
			*anim.second.flag = true;
			animator = anim.second.animation;
		}
		else if (animID != anim.first && *anim.second.flag) {
			*anim.second.flag = false;
		}
	}
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