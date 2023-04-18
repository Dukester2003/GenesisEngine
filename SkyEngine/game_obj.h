#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "g_model.h"
#include "g_shader.h"


class GameObject
{
	public:
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objmodel);
		GameObject();
		void DrawModel(Model modelRender,Shader modelShader);
		btCollisionObject* collisionObject;
		btRigidBody* rigidBody;
		btMotionState* motionState;
		glm::vec3 Position, Size, Velocity;
		glm::quat Rotation;
		void GLPosToBulletPos();
		

		string Name;
		bool IsSelected;
		bool Destroyed;

		Model model;
};

#endif // !GAMEOBJECT