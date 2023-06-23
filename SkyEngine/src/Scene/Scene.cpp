#include "Scene.h"
#include "../CollisionShapes/BaseShape.h"




Scene::Scene()
{
    dirLight = make_unique<DirectionalLight>();
}

Scene::~Scene()
{
    // destructor implementation
}

void Scene::InitShaders()
{
    // build and compile our shaders program
    // ------------------------------------
    gridShader = Shader("shaders/grid.vs", "shaders/grid.fs");
    modelShader = Shader("shaders/model.vs", "shaders/model.fs");
    diffuseShader = Shader("shaders/diffuse.vs", "shaders/diffuse.fs");
    animationShader = Shader("shaders/anim.vs", "shaders/anim.fs");
    eulerShader = Shader("shaders/euler.vs", "shaders/euler.fs");
    collisionShader = Shader("shaders/collisionShader.vs", "shaders/collisionShader.fs");
}


void Scene::CreateShaderTransformations()
{
    // create transformations
    animationShader.setMat4("projection", projection);
    animationShader.setMat4("view", view);
    gridShader.use();
    gridShader.setMat4("projection", projection);
    gridShader.setMat4("view", view);
    collisionShader.use();
    collisionShader.setMat4("projection", projection);
    collisionShader.setMat4("view", view);
    diffuseShader.use();
    diffuseShader.setMat4("projection", projection);
    diffuseShader.setMat4("view", view);
    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
}

void Scene::SetPerspectiveTransformations(const float screenWidth, const float screenHeight)
{
    projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    view = camera.GetViewMatrix();
}

void Scene::UpdateObjects(Shader& shader, btDynamicsWorld* dynamicsWorld)
{
    for (auto& item : items) { item->UpdateObject(shader, dynamicsWorld); }
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
            items.push_back(std::make_shared<Box>(position, scale, rotation));
        }
        if (type == "Sphere") {
            items.push_back(std::make_shared<Sphere>(position, scale, rotation));
        }
        if (type == "Cylinder") {
            items.push_back(std::make_shared<Cylinder>(position, scale, rotation));
        }
        if (type == "Capsule") {
            items.push_back(std::make_shared<Capsule>(position, scale, rotation));
        }
        if (type == "Cone") {
            items.push_back(std::make_shared<Cone>(position, scale, rotation));
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

///
/// For the lights in this scene,
/// 

void Scene::UpdateDirLights(Shader& shader)
{
    shader.setVec3("dirLight.direction",  dirLight->Direction);
    shader.setVec3("dirLight.ambient",    dirLight->Ambient);
    shader.setVec3("dirLight.diffuse",    dirLight->Diffuse);
    shader.setVec3("dirLight.specular",   dirLight->Specular);
}

void Scene::UpdatePointLights(Shader& shader)
{
    for (int i = 0; i < pointLights.size(); ++i)
    {
        std::string lightName = "pointLights[" + std::to_string(i) + "]";
        if (!pointLights[i].modelDrawn) { pointLights[i].InitGizmo(); pointLights[i].modelDrawn = true; }
        if (enableGizmos) { pointLights[i].DrawGizmo(shader); }
        shader.setVec3(lightName + ".position", pointLights[i].Position);
        shader.setVec3(lightName + ".ambient", pointLights[i].Ambient);
        shader.setVec3(lightName + ".diffuse", pointLights[i].Diffuse);
        shader.setVec3(lightName + ".specular", pointLights[i].Specular);
        shader.setFloat(lightName + ".constant", pointLights[i].Constant);
        shader.setFloat(lightName + ".linear", pointLights[i].Linear);
        shader.setFloat(lightName + ".quadratic", pointLights[i].Quadratic);
        shader.setBool(lightName + ".blinn", pointLights[i].Blinn);
    }
}


// SpotLights
void Scene::UpdateSpotLights(Shader& shader)
{
    for (int i = 0; i < spotLights.size(); ++i)
    {
        std::string lightName = "spotLights[" + std::to_string(i) + "]";

        shader.setVec3(lightName + ".position",  spotLights[i].Position);
        shader.setVec3(lightName + ".ambient",   spotLights[i].Ambient);
        shader.setVec3(lightName + ".diffuse",   spotLights[i].Diffuse);
        shader.setVec3(lightName + ".specular",  spotLights[i].Specular);
        shader.setFloat(lightName + ".constant", spotLights[i].Constant);
        shader.setFloat(lightName + ".linear",   spotLights[i].Linear);
        shader.setFloat(lightName + ".quadratic",spotLights[i].Quadratic);
        shader.setFloat(lightName + ".cutOff", glm::cos(glm::radians(spotLights[i].CutOff)));
        shader.setFloat(lightName + ".outerCutOff", glm::cos(glm::radians(spotLights[i].OuterCutOff)));
    }
}