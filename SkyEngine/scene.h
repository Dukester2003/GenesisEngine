#ifndef SCENE
#define SCENE

#include "game_obj.h"
#include "player.h"
#include "g_camera.h"
#include <vector>
#include <list>
#include <fstream>

#include <json.hpp>

using json = nlohmann::json;
inline glm::vec3 startingSpawnPosition = glm::vec3(0.0f, 5.0f, 0.0f);
inline btAlignedObjectArray<btCollisionShape*> collisionShapes;
inline std::vector<std::shared_ptr<GameObject>> items;
inline std::shared_ptr<GameObject> copiedObject;
inline size_t itemIndex;
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


class Scene
{   
    public:
    // Function to save the current scene
        void SaveScene(const std::string& filename, const std::vector<std::shared_ptr<GameObject>>& items);

    // Function to load a scene
		void LoadScene(const std::string& filename, std::vector<std::shared_ptr<GameObject>>& items);
    
};




#endif // SCENE

