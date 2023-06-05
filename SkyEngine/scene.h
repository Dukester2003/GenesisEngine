#ifndef SCENE
#define SCENE

#include "light.h"
#include "game_obj.h"
#include "player.h"
#include "g_camera.h"
#include <vector>
#include <list>
#include <fstream>

#include <json.hpp>

using json = nlohmann::json;
inline glm::vec3 spawnPosition = glm::vec3(0.0f, 5.0f, 0.0f);
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
	Scene();
	~Scene();
	// Function to save the current scene
    void SaveScene(const std::string& filename, const std::vector<std::shared_ptr<GameObject>>& items);

	// Function to load a scene
	void LoadScene(const std::string& filename, std::vector<std::shared_ptr<GameObject>>& items);


	// Function to get the files to load in the directory
	std::vector<std::string> getFilesInDirectory(const std::string& directory); 

	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> dirLights;
	std::vector<SpotLight> spotLights;

	void DefaultPointLights(Shader& shader);
	void ActivatePointLights(Shader& shader);
	void UpdatePointLights(Shader& shader);

	void DefaultDirLights(Shader& shader);
	void ActivateDirLights(Shader& shader);
	void UpdateDirLights(Shader& shader);

	void DefaultSpotLights(Shader& shader);
	void ActivateSpotLights(Shader& shader);
	void UpdateSpotLights(Shader& shader);

	bool dirLightPresent;
	bool pointLightPresent;
	bool spotLightPresent;
}; 




#endif // SCENE

