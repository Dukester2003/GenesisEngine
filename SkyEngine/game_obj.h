#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "g_model.h"
#include "g_shader.h"

enum class ShapeType
{
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	CONE
};

enum class BlockType
{
	GRASS,
	STONE,
};

class GameObject 
{

	public:
		// Constructers that uses quaternion roation,
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objmodel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::quat rotation);

		// Constructers that allows for euler angles, though quaternions are more recommended
		// eulers should only be used for less computationally expensive tasks, UI design, and very simple rotations.
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation, Model objmodel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 eulerRotation);

		GameObject();
		virtual ~GameObject() = default;
		ShapeType type;
		BlockType blockType;
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
		glm::vec3 getScale() const { return Size; }
		ShapeType getType() const { return type; }
		float getMass() const { return massValue; }
		float getFrictionValue() const { return frictionValue; }
		btVector3 getLocalIntertia() const { return localInertia; }
		virtual Shader getShader() const { return Shader(); };

		// Setters
		void setPosition(const glm::vec3& newPosition) { Position = newPosition; }
		void setRotation(const glm::quat& newRotation) { Rotation = newRotation; }
		void setVelocity(const glm::vec3& newVelocity) { Velocity = newVelocity; }
		void setSize(const glm::vec3& newSize) { Size = newSize; }
		virtual void setShader() {}


		void copy();
		void paste();

		
		
		virtual void InitModel() {}
		void DrawModel(Model modelRender,Shader modelShader);
		virtual std::shared_ptr<GameObject> clone() const { return NULL; };
		virtual void InitiateRigidBody(btDynamicsWorld* dynamicsWorld) {}
		virtual void UpdateRigidBody() {}
		virtual void InitiateGUI() {}
		virtual void UpdateObject(Model model, Shader shader, btDynamicsWorld* dynamicsWorld);
		virtual void ObjMenu(string name);
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