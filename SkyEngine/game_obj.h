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
		void ObjMenu(std::string name);
		btCollisionShape* collisionShape;
		btCollisionObject* collisionObject;
		btRigidBody* rigidBody;
		btMotionState* motionState;
		glm::vec3 Position, Size, Velocity;
		glm::quat Rotation;
		void GLPosToBulletPos();
		int objID;
		bool isDynamic;
		// mass of gameObject, 0 mass means the object will be static.
		btScalar massValue;
		btVector3 localInertia;
		float frictionValue;

		string Name;
		bool IsSelected;
		bool Destroyed;

		Model model;
};

#endif // !GAMEOBJECT