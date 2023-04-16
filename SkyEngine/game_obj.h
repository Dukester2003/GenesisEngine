#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "g_model.h"
#include "g_shader.h"


class GameObject
{
	public:
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 rotation, Model objmodel);
		GameObject();
		void DrawModel(Model modelRender,Shader modelShader);
		btCollisionObject* collisionObject;
		btRigidBody* rigidBody;
		btMotionState* motionState;
		glm::vec3 Position, Size, Velocity, Rotation;
		glm::quat quaternion;
		void GLPosToBulletPos();
		void rotate(float angle, glm::vec3& axis) {
			glm::quat rotationDelta = glm::angleAxis(glm::radians(angle), axis);
			quaternion = rotationDelta * quaternion;
		}
		

		string Name;
		bool IsSelected;
		bool Destroyed;

		Model model;
};

#endif // !GAMEOBJECT