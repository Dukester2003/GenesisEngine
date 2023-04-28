#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "g_model.h"
#include "g_shader.h"

enum CollisionShapeTypes
{
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	CONE
};



class GameObject
{
	public:
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objmodel);
		GameObject();
		~GameObject();
		static int collisionShapeType;
		btDynamicsWorld* _dynamicsWorld;
		btCollisionShape* collisionShape;
		btCollisionObject* collisionObject;
		btRigidBody* rigidBody;
		btMotionState* motionState;
		glm::vec3 Position, Size, Velocity;
		glm::quat Rotation;		
		bool rigidBodyEnabled;
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

	public:
		glm::vec3 getPosition() const { return Position; }
		void setPosition(const glm::vec3& newPosition) { Position = newPosition; }
		void DrawModel(Model modelRender,Shader modelShader);
		virtual void ObjMenu(std::string name);
		void setRigidBodyEnabled(bool enabled);
		void updateSize(const btVector3& newSize);
		void ScaleUniform(const char* label, float* values, float speed, float min_value, float max_value);
		virtual void createCollisionShape() { }
		void GLPosToBulletPos();
};

#endif // !GAMEOBJECT