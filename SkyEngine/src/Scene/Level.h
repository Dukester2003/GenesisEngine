#ifndef LVL_H
#define LVL_H

#include "../Core/Common/Model.h"
#include "../Core/Object/GameObject.h"
#include "../CollisionShapes/TriangleMesh.h"
class Level
{
	public:
		Level(Model lvlModel);
		Level();
		~Level();
		Model model;
		Material material;
		TriangleMesh* triangleMesh;
		void InitModel();
		void DrawLevel(Shader shader);
		void IterateMeshes();
		void InitCollision(btDynamicsWorld* dynamicsWorld ,btAlignedObjectArray<btCollisionShape*> collisionShapes);
		void UpdateCollision(btDynamicsWorld* dynamicsWorld);
};

#endif // !LVL_H
