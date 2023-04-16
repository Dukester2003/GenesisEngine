#ifndef SCENE
#define SCENE

#include "game_obj.h"
#include "player.h"
#include <vector>

inline btAlignedObjectArray<btCollisionShape*> collisionShapes;
inline std::vector<GameObject> items;
inline Player* player;



#endif // SCENE

