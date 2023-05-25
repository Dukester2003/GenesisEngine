#include "scene.h"
#include "g_collision.h"



Scene::Scene()
{
    // constructor implementation
}

Scene::~Scene()
{
    // destructor implementation
}

void Scene::SaveScene(const std::string& filename, const std::vector<std::shared_ptr<GameObject>>& items) {
    json j;

    for (const auto& item : items) {
        json itemJson;
        itemJson["type"] = item->getType();
        itemJson["position"] = { item->getPosition().x, item->getPosition().y, item->getPosition().z };
        itemJson["scale"] = { item->getScale().x, item->getScale().y, item->getScale().z };

        if(item->hasVelocity) {itemJson["velocity"] = { item->getVelocity().x, item->getVelocity().y, item->getVelocity().z };}   

        if (!item->isEuler) {
            glm::quat rotation = item->getRotation();
            itemJson["rotation"] = { rotation.w, rotation.x, rotation.y, rotation.z };
        }
        else {
            glm::vec3 eulerRotation = item->getEulerRotation();
            itemJson["eulerRotation"] = { eulerRotation.x, eulerRotation.y, eulerRotation.z };
        }
        
       
        j.push_back(itemJson);
    }
    std::string path = "..\\..\\SkyEngine\\SkyEngine\\saves\\" + std::string(filename);
    std::ofstream o(path);

    if (!o) {
        // Handle the error here.
        // For example, print an error message and exit the function.
        std::cerr << "Error: Could not open file for writing: " << path << '\n';
        return;
    }

    o << std::setw(4) << j << std::endl;
}

// Function to load a scene
void Scene::LoadScene(const std::string& filename, std::vector<std::shared_ptr<GameObject>>& items) {
    std::string path = "..\\..\\SkyEngine\\SkyEngine\\saves\\" + std::string(filename);
    std::ifstream i(path);
    json j;
    i >> j;

    if (!i) {
        // Handle the error here.
        // For example, print an error message and exit the function.
        std::cerr << "Error: Could not open file for reading: " << path << '\n';
        return;
    }

    

    items.clear();

    for (const auto& itemJson : j) {
        glm::vec3 position = glm::vec3(itemJson["position"][0], itemJson["position"][1], itemJson["position"][2]);
        glm::vec3 scale = glm::vec3(itemJson["scale"][0], itemJson["scale"][1], itemJson["scale"][2]);
        glm::quat rotation = glm::quat(itemJson["rotation"][0], itemJson["rotation"][1], itemJson["rotation"][2], itemJson["rotation"][3]);
        glm::vec3 velocity = glm::vec3(itemJson["velocity"][0], itemJson["velocity"][1], itemJson["velocity"][2]);
        std::string type = itemJson["type"];
        if (type == "Box") {
            items.push_back(std::make_shared<BoxCollider>(position, scale, rotation));
        }
        if (type == "Sphere") {
            items.push_back(std::make_shared<SphereCollider>(position, scale, rotation));
        }
        if (type == "Cylinder") {
            items.push_back(std::make_shared<CylinderCollider>(position, scale, rotation));
        }
        if (type == "Capsule") {
            items.push_back(std::make_shared<CapsuleCollider>(position, scale, rotation));
        }
        if (type == "Cone") {
            items.push_back(std::make_shared<ConeCollider>(position, scale, rotation));
        }
        // add other types as necessary
    }
}
std::vector<std::string> Scene::getFilesInDirectory(const std::string& directory) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        files.push_back(entry.path().string());
    }
    return files;
}