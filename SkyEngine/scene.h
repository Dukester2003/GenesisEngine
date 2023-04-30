#ifndef SCENE
#define SCENE

#include "game_obj.h"
#include "player.h"
#include "g_camera.h"
#include <vector>

inline btAlignedObjectArray<btCollisionShape*> collisionShapes;
inline std::vector<GameObject> items;
inline int itemIndex;
inline Player* player;
inline Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
inline bool enableVisibleColliders;
inline bool canPaste;

struct Copy
{
	glm::vec3 copyPosition; 
	glm::quat copyRotation; 
	glm::vec3 copyVelocity; 
	glm::vec3 copySize; 
	float copyMass; 
	float copyFriction; 
	btVector3 copyInertia;
};

inline Copy* _copy = new Copy;

#endif // SCENE

