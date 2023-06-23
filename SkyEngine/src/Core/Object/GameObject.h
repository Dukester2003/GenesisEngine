#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "../../../Model.h"
#include "../../../Shader.h"
#include "../../../Light.h"

enum class ShapeType
{
	PLANE,
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	CONE,
	COMPOUND,
	CONVEXHULL,
	TRIANGLEMESH,
	HEIGHTFIELD,
	SOFTBODY,
	MULTISPHERE,
	CONVEX_POINT_CLOUD
};

enum class BlockType
{
	GRASS,
	STONE,
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material() : ambient(0.3f), diffuse(0.3f), specular(0.3f), shininess(32.0f) {}
	glm::vec3 getAmbient() { return ambient; }
	glm::vec3 getDiffuse() { return diffuse; }
	glm::vec3 getSpecular() { return specular; }
	float getShininess() { return shininess; }

	void setAmbient(glm::vec3 newAmbient) { ambient = newAmbient; }
	void setDiffuse(glm::vec3 newDiffuse) { diffuse = newDiffuse; }
	void setSpecular(glm::vec3 newSpecular) { specular = newSpecular; }
	void setShininess(float newShininess) { shininess = newShininess; }
};
class GameObject 
{

	public:
		// Constructers that uses quaternion roation,
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation, Model objModel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::quat rotation);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::quat rotation, Model objModel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::quat rotation);		


		// Constructers that allows for euler angles, though quaternions are more recommended
		// eulers should only be used for less computationally expensive tasks, UI design, and very simple rotations.
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation, Model objmodel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 velocity, glm::vec3 eulerRotation);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 eulerRotation, Model objmodel);
		GameObject(glm::vec3 pos, glm::vec3 size, glm::vec3 eulerRotation);	

		// A non-rotating, non-scalable constructer, best for light sources.
		GameObject(glm::vec3 pos);
		// Okay... moving light sources exist too ya know.
		GameObject(glm::vec3 pos, glm::vec3 velocity);
		GameObject();
		
		virtual ~GameObject() {};
		// ...
		std::shared_ptr<Light> lightSource;  // Light emitted by this object, if any
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
		bool hasVelocity;
		bool isEuler;
		bool modelDynamic;
		// mass of gameObject, 0 mass means the object will be static.
		btScalar massValue;
		btVector3 localInertia;
		float frictionValue;
		Material material;
		string Name;
		bool IsSelected;
		bool Destroyed;

		Model model;

	public:
		// Getters
		btCollisionShape* getBtCollisionShape() const { return collisionShape; }
		btCollisionObject* getBtCollisionObject() const { return collisionObject; }
		glm::vec3 getPosition() const { return Position; }
		glm::quat getRotation() const { return Rotation; }
		glm::vec3 getEulerRotation() const { return glm::eulerAngles(Rotation); }
		glm::vec3 getVelocity() const { return Velocity; }
		glm::vec3 getScale() const { return Size; }
		ShapeType getType() const { return type; }
		float getMass() const { return massValue; }
		float getFrictionValue() const { return frictionValue; }
		btVector3 getLocalIntertia() const { return localInertia; }
		virtual Shader getShader() const { return Shader(); };
		Model getModel() const { return model; }

		// Setters
		void setBtCollisionShape(btCollisionShape* newCollisionShape) { collisionShape = newCollisionShape; }
		void setBtCollisionObject(btCollisionObject* newCollisionObject) { collisionObject = newCollisionObject; }
		void setPosition(const glm::vec3& newPosition) { Position = newPosition; }
		void setRotation(const glm::quat& newRotation) { Rotation = newRotation; }
		void setVelocity(const glm::vec3& newVelocity) { Velocity = newVelocity; }
		void setScale(const glm::vec3& newSize) { Size = newSize; }
		void setType(const ShapeType newType) { type = newType; }
		void setFrictionValue(const float newFrictionValue);
		virtual void setShader() {}

		
		
		virtual void InitModel() {}
		void DrawModel(Model modelRender, Shader modelShader);

		void DrawModel(Shader shader);
		virtual std::shared_ptr<GameObject> clone() const { return NULL; };
		virtual void InitiateRigidBody(btDynamicsWorld* dynamicsWorld, btAlignedObjectArray<btCollisionShape*> collisionShapes) {}
		virtual void UpdateRigidBody() {}
		virtual void InitiateGUI() {}
		virtual void UpdateObject(Shader shader, btDynamicsWorld* dynamicsWorld);
		virtual void ObjMenu(string name);
		void setRigidBodyEnabled(bool enabled);
		void updateSize(const btVector3& newSize);
		void setMass(float newMass);
		void ScaleUniform(const char* label, float* values, float speed, float min_value, float max_value);		
		virtual void createCollisionShape() { }
};

#endif // !GAMEOBJECT