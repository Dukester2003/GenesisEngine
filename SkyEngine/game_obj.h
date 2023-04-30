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
		// Getters
		glm::vec3 getPosition() const { return Position; }
		glm::quat getRotation() const { return Rotation; }
		glm::vec3 getVelocity() const { return Velocity; }
		glm::vec3 getSize() const { return Size; }
		float getMass() const { return massValue; }
		float getFrictionValue() const { return frictionValue; }
		btVector3 getLocalIntertia() const { return localInertia; }

		// Setters
		void setPosition(const glm::vec3& newPosition) { Position = newPosition; }
		void setRotation(const glm::quat& newRotation) { Rotation = newRotation; }
		void setVelocity(const glm::vec3& newVelocity) { Velocity = newVelocity; }
		void setSize(const glm::vec3& newSize) { Size = newSize; }
		void copy();
		void paste();

		void DrawModel(Model modelRender,Shader modelShader);
		virtual void ObjMenu(std::string name);
		void setRigidBodyEnabled(bool enabled);
		void updateSize(const btVector3& newSize);
		void setMass(float newMass);
		void updatePosition(const btVector3& newPosition);
		void updateRotation(const btQuaternion& newRotation);
		void ScaleUniform(const char* label, float* values, float speed, float min_value, float max_value);		
		virtual void createCollisionShape() { }
		void GLPosToBulletPos();
};

#endif // !GAMEOBJECT