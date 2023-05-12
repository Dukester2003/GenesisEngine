#pragma once
#ifndef G_GUI_H
#define G_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "Common_Assets.h"
#include "init_collision.h"

#include <dirent.h>
#include <sys/stat.h>
#include <filesystem> // Include this for std::filesystem::path
ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse; // by ex
ImGuiWindowFlags sceneFlags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollWithMouse;
btVector3 _btPosition = btVector3(0.0f, 0.0f, 0.0f);
bool pushedBack;
bool show_file_explorer;
std::string current_path = "."; // Set an initial path, e.g., the current working directory
void FileExplorer(const std::string& title, std::string& current_path);
bool isSceneHovered = false; 
void imguiSetup(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}
void GUI_INIT();

bool IsMouseHoveringOverWindow(const char* window_name)
{
    // Get the window by name
    ImGui::SetNextWindowFocus();
    if (ImGui::Begin(window_name, nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs))
    {
        ImGui::End();
        return false; // If the window is not found, the mouse is not hovering over it
    }

    // Check if the mouse is hovering over the current window
    bool is_hovered = ImGui::IsWindowHovered();

    ImGui::End();

    return is_hovered;
}

bool ListFiles(const std::string& path, std::vector<std::string>& files, std::vector<std::string>& directories) {
    DIR* dir;
    struct dirent* ent;
    struct stat sb;

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string entry_name = ent->d_name;
            std::string full_path = path + "/" + entry_name;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                if (entry_name != "." && entry_name != "..") {
                    directories.push_back(entry_name);
                }
            }
            else {
                files.push_back(entry_name);
            }
        }
        closedir(dir);
        return true;
    }
    return false;
}

void FileExplorer(const std::string& title, std::string& current_path) {
    ImGui::Begin(title.c_str());

    // Change directory by clicking on the folder name
    if (ImGui::BeginCombo("Current Directory", current_path.c_str())) {
        std::vector<std::string> directories;
        ListFiles(current_path, std::vector<std::string>(), directories);
        for (const std::string& dir : directories) {
            if (ImGui::Selectable(dir.c_str())) {
                current_path += "/" + dir;
            }
        }
        ImGui::EndCombo();
    }

    // List files and directories
    ImGui::Text("Files:");
    ImGui::Separator();
    if (ImGui::Button("Go Up")) {
        if (current_path != ".") {
            std::filesystem::path parent_path = std::filesystem::path(current_path).parent_path();
            current_path = parent_path.string();
        }
        
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) { show_file_explorer = false; }
    ImGui::BeginChild("FileList", ImVec2(0, ImGui::GetWindowHeight() * 0.75f), true);

    std::vector<std::string> files, directories;
    if (ListFiles(current_path, files, directories)) {
        for (const std::string& dir : directories) {
            ImGui::Text("[DIR] %s", dir.c_str());
        }
        for (const std::string& file : files) {
            ImGui::Text("%s", file.c_str());
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void AddItem(std::shared_ptr<GameObject> item, btDynamicsWorld* dynamicsWorld) {
    item->massValue = 5.0f;
    item->InitiateRigidBody(dynamicsWorld);
}

inline void GUI_INIT()
{



    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            {
                // Set a flag to show the file explorer
                show_file_explorer = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {

            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) { /*menuActive = false; */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do Things */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do Things */ }
            if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Do Things */ }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Do Things */ }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Do Things */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Quality")) { /*Do Things */ }
            if (ImGui::MenuItem("Toggle Grid", "Ctrl+G")) {}
            if (ImGui::MenuItem("Scene Options"))
            {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Box"))
            {
                auto boxCollider = std::make_shared<BoxCollider>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), boxModel);               
                AddItem(boxCollider, dynamicsWorld);
                items.push_back(boxCollider);
            }

            if (ImGui::MenuItem("Sphere"))
            {
                auto sphereCollider = std::make_shared<SphereCollider>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), sphereModel);
                AddItem(sphereCollider, dynamicsWorld);
                items.push_back(sphereCollider);
            }

            if (ImGui::MenuItem("Cylinder"))
            {
                auto cylinderCollider = std::make_shared<CylinderCollider>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), cylinderModel);
                AddItem(cylinderCollider, dynamicsWorld);
                items.push_back(cylinderCollider);
            }

            if (ImGui::MenuItem("Capsule"))
            {
                auto capsuleCollider = std::make_shared<CapsuleCollider>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), capsuleModel);
                AddItem(capsuleCollider, dynamicsWorld);
                items.push_back(capsuleCollider);
            }

            if (ImGui::MenuItem("Cone"))
            {
                auto coneCollider = std::make_shared<ConeCollider>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f) , coneModel);
                AddItem(coneCollider, dynamicsWorld);
                items.push_back(coneCollider);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (show_file_explorer) {
        FileExplorer("File Explorer", current_path);
    }

    ImGui::Begin("Assets", NULL, flags);
    ////////////////////////////////////////////////////////
    ///                   SPAWN MENU                     ///
    ////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Objects"))
    {
        if (ImGui::TreeNode("Platforms"))
        {
            if (ImGui::TreeNode("sb"))
            {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader("Entities"))
    {
        /// <summary>
        /// Enemies
        /// </summary>
        /// <returns></returns>
        if (ImGui::TreeNode("Enemies"))
        {

            if (ImGui::Button("Big Bob-Omb"))
            {

            }
            if (ImGui::Button("Big Goomba"))
            {
                /*
                items.push_back(Goomba());
                bigGoombas.push_back(Goomba(glm::vec3(0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), goombaModel));
                */
            }
            if (ImGui::Button("Bomb-Omb"))
            {
                /*
                bobOmbs.push_back(BobOmb(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 45.0f, 0.0f), bobOmbModel));
                items.push_back(BobOmb());
                for (auto& bobOmb : bobOmbs)
                {

                }
                */
            }
            if (ImGui::Button("Boo"))
            {

            }
            if (ImGui::Button("Chain-Chomp"))
            {

            }
            if (ImGui::Button("Goomba"))
            {
                /*
                items.push_back(Goomba());
                goombas.push_back(Goomba(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), goombaModel));
                */
            }
            if (ImGui::Button("Hammer Bro"))
            {

            }
            if (ImGui::Button("Koopa"))
            {

            }
            if (ImGui::Button("Piranha Plant"))
            {

            }
            if (ImGui::Button("Shy Guy"))
            {

            }
            if (ImGui::Button("Thwomp"))
            {

            }
            if (ImGui::Button("Whomp"))
            {
                /*
                items.push_back(Whomp());
                whomps.push_back(Whomp(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), whompModel));
                */
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Bosses"))
        {
            if (ImGui::Button("Bob-Omb King"))
            {

            }
            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader("Level Elements"))
    {
        if (ImGui::TreeNode("Terrain"))
        {
            if (ImGui::Button("Grass Block"))
            {
                auto grassBlock = std::make_shared<GrassBlock>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), grassBlockModel);
                AddItem(grassBlock, dynamicsWorld);
                items.push_back(grassBlock);
            }
            if (ImGui::Button("Stone Block"))
            {

            }
            if (ImGui::Button("Grass Ramp"))
            {

            }
            if (ImGui::Button("Wood Crate"))
            {

            }
            ImGui::TreePop();
        }

    }
    if (ImGui::CollapsingHeader("Misc"))
    {

    }
    ImGui::End();

    static int current_index = 0;

    ImGui::Begin("Objects In Scene", NULL, flags);
    if (ImGui::Button("Play"))
    {

    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {

    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {

    }
    if (ImGui::Button("Paste"))
    {
        if (copiedObject) {
            items.push_back(copiedObject->clone());
        }
    }
    if (ImGui::BeginListBox("Objects in Scene:", ImVec2(250, 1500)))
    {
        // Add counters for each shape type
        int boxCounter = 0;
        int sphereCounter = 0;
        int cylinderCounter = 0;
        int capsuleCounter = 0;
        int coneCounter = 0;

        static int selectedItem = 0;
        itemIndex = 0;
        for (auto& item : items)
        {
            switch (item->type) {
            case ShapeType::BOX:
               item->Name = "Box " + std::to_string(static_cast<BoxCollider*>(item.get())->id);
                break;
            case ShapeType::SPHERE:
                item->Name = "Sphere " + std::to_string(static_cast<SphereCollider*>(item.get())->id);
                break;
            case ShapeType::CYLINDER:
                item->Name = "Cylinder " + std::to_string(static_cast<CylinderCollider*>(item.get())->id);
                break;
            case ShapeType::CAPSULE:
                item->Name = "Capsule " + std::to_string(static_cast<CapsuleCollider*>(item.get())->id);
                break;
            case ShapeType::CONE:
                item->Name = "Cone " + std::to_string(static_cast<ConeCollider*>(item.get())->id);
                break;
            default:
                item->Name = "Unkown";
                break;
                // Add more cases for other shape types if needed
            }

            switch (item->blockType) {
                case BlockType::GRASS:
                    item->Name = "GrassBlock " + std::to_string(static_cast<GrassBlock*>(item.get())->id);
            }
            item->IsSelected = (selectedItem == itemIndex);
            if (ImGui::Selectable(item->Name.c_str(), item->IsSelected)) {
                selectedItem = itemIndex;
            }
            if (item->IsSelected) {
                // Call the item menu function for the selected object
                item->ObjMenu(item->Name);
            }
            itemIndex++;
            // ... Use the 'name' string to display the object in ImGui ...

            
        }

        if (ImGui::Button("Copy"))
        {
            if (selectedItem >= 0 && selectedItem < items.size()) {
                copiedObject = items[selectedItem]->clone();
            }
        }
        ImGui::EndListBox();
        // After the ImGui::EndListBox() line
        
    }
    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


#endif