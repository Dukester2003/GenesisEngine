#include "scene.h"
#include "g_collision.h"

void Scene::SaveScene(const std::string& filename, const std::vector<std::shared_ptr<GameObject>>& items) {
    json j;

    for (const auto& item : items) {
        json itemJson;
        itemJson["type"] = item->getType();
        itemJson["position"] = { item->getPosition().x, item->getPosition().y, item->getPosition().z };
        itemJson["scale"] = { item->getScale().x, item->getScale().y, item->getScale().z };

        glm::quat rotation = item->getRotation();
        itemJson["rotation"] = { rotation.w, rotation.x, rotation.y, rotation.z };

        j.push_back(itemJson);
    }

    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}

// Function to load a scene
void Scene::LoadScene(const std::string& filename, std::vector<std::shared_ptr<GameObject>>& items) {
    std::ifstream i(filename);
    json j;
    i >> j;

    items.clear();

    for (const auto& itemJson : j) {
        glm::vec3 position = glm::vec3(itemJson["position"][0], itemJson["position"][1], itemJson["position"][2]);
        glm::vec3 scale = glm::vec3(itemJson["scale"][0], itemJson["scale"][1], itemJson["scale"][2]);
        glm::quat rotation = glm::quat(itemJson["rotation"][0], itemJson["rotation"][1], itemJson["rotation"][2], itemJson["rotation"][3]);

        std::string type = itemJson["type"];
        if (type == "Box") {
            items.push_back(std::make_shared<BoxCollider>(position, scale, rotation));
        }
        // add other types as necessary
    }
}