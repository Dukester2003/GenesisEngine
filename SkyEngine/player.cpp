#include "player.h"
#include <GLFW/glfw3.h>


using namespace irrklang;

Player::Player()
	: GameObject() {}
Player::Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel)
	: GameObject(pos, size, velocity, rotation, objModel) {}
Player::Player(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation)
	: GameObject(pos, size, velocity, rotation) {}

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