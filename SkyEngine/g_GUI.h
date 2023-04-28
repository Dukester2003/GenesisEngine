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
#include "surface_collision.h"

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
void GUI_INIT()
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
            if (ImGui::MenuItem("Toggle Grid", "Ctrl+G")) {  }
            if (ImGui::MenuItem("Scene Options"))
            {
                
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Box"))
            {
                
                boxColliders.push_back(BoxCollider(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), boxModel));
                static int lastProcessedIndex = -1;
                // Process only newly added BoxColliders
                for (int i = lastProcessedIndex + 1; i < boxColliders.size(); ++i) {
                    auto& boxCollider = boxColliders[i];
                    boxCollider.massValue = 5.0f;
                    boxCollider.InitiateRigidBody(dynamicsWorld);                 
                }

                lastProcessedIndex = boxColliders.size() - 1;
            }

            if (ImGui::MenuItem("Sphere"))
            {            
                sphereColliders.push_back(SphereCollider(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), sphereModel));
                static int lastProcessedIndex = -1;
                // Process only newly added SphereColliders
                for (int i = lastProcessedIndex + 1; i < sphereColliders.size(); ++i) {
                    auto& sphereCollider = sphereColliders[i];
                    sphereCollider.massValue = 5.0f;
                    sphereCollider.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = sphereColliders.size() - 1;
                
            }

            if (ImGui::MenuItem("Cylinder"))
            {
                cylinderColliders.push_back(CylinderCollider(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), cylinderModel));
                static int lastProcessedIndex = -1;
                // Process only newly added CylinderColliders
                for (int i = lastProcessedIndex + 1; i < cylinderColliders.size(); ++i) {
                    auto& cylinderCollider = cylinderColliders[i];
                    cylinderCollider.massValue = 5.0f;
                    cylinderCollider.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = cylinderColliders.size() - 1;
            }

            if (ImGui::MenuItem("Capsule"))
            {
                capsuleColliders.push_back(CapsuleCollider(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), capsuleModel));
                static int lastProcessedIndex = -1;
                // Process only newly added CapsuleColliders
                for (int i = lastProcessedIndex + 1; i < capsuleColliders.size(); ++i) {
                    auto& capsuleCollider = capsuleColliders[i];
                    capsuleCollider.massValue = 5.0f;
                    capsuleCollider.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = capsuleColliders.size() - 1;
            }

            if (ImGui::MenuItem("Cone"))
            {
                coneColliders.push_back(ConeCollider(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), coneModel));
                static int lastProcessedIndex = -1;
                // Process only newly added ConeColliders
                for (int i = lastProcessedIndex + 1; i < coneColliders.size(); ++i) {
                    auto& coneCollider = coneColliders[i];
                    coneCollider.massValue = 5.0f;
                    coneCollider.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = coneColliders.size() - 1;
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
                grassBlocks.push_back(GrassBlock(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(0.0f), grassBlockModel));
                static int lastProcessedIndex = -1;
                // Process only newly added BoxColliders
                for (int i = lastProcessedIndex + 1; i < grassBlocks.size(); ++i) {
                    auto& grassBlock = grassBlocks[i];
                    grassBlock.massValue = 0.0f;
                    grassBlock.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = grassBlocks.size() - 1;
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
    if (ImGui::BeginListBox("Objects in Scene:", ImVec2(250, 1500)))
    {
        for (itemIndex = 0;auto& item : items)
        {
            itemIndex++;
            
            // Spawn Data for Player
            player->Name = "Player";
            player->IsSelected = (current_index == itemIndex);
            if (ImGui::Selectable(player->Name.c_str(), player->IsSelected)) { current_index = itemIndex; }
            if (player->IsSelected)
            {
                ImGui::SetItemDefaultFocus();
                // Bring this menu up if object is selected

                ImGui::Begin(player->Name.c_str());

                if (ImGui::BeginChild("Child Window", ImVec2(500, 20), false))
                {
                    ImGui::DragFloat3("Object Pos", (float*)&player->Position);
                    ImGui::EndChild();
                }
                ImGui::End();  // handle selection


            }

            for (int n = 0; auto & box : boxColliders)
            {                          
                box.Name = "Box";
                std::string _name = box.Name + std::to_string(n);
                box.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), box.IsSelected)) { current_index = itemIndex;}
                if (box.IsSelected) {box.ObjMenu(_name); }
                itemIndex++;   
                n++;
            }

            for (int n = 0; auto & sphere : sphereColliders)
            {             
                sphere.Name = "Sphere";
                std::string _name = sphere.Name + std::to_string(n);
                sphere.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), sphere.IsSelected)) { current_index = itemIndex;}
                if (sphere.IsSelected) { sphere.ObjMenu(_name); }
                itemIndex++;
                n++;
            }

            for (int n = 0; auto & cylinder : cylinderColliders)
            {                        
                cylinder.Name = "Cylinder";
                std::string _name = cylinder.Name + std::to_string(n);
                cylinder.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), cylinder.IsSelected)) { current_index = itemIndex; }
                if (cylinder.IsSelected) { cylinder.ObjMenu(_name);}
                itemIndex++;   
                n++;
            }

            for (int n = 0; auto & capsule : capsuleColliders)
            {        
                capsule.Name = "Capsule";
                std::string _name = capsule.Name + std::to_string(n);
                capsule.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), capsule.IsSelected)) { current_index = itemIndex; }
                if (capsule.IsSelected) { capsule.ObjMenu(_name); }
                itemIndex++;
                n++;
            }

            for (int n = 0; auto & cone : coneColliders)
            {                
                cone.Name = "Cone";
                std::string _name = cone.Name + std::to_string(n);
                cone.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), cone.IsSelected)) { current_index = itemIndex;}
                if (cone.IsSelected) { cone.ObjMenu(_name); }
                itemIndex++;
                n++;
            }
            for (int n = 0; auto & grassBlock : grassBlocks)
            {
                itemIndex++;
                grassBlock.Name = "Grass Block";
                std::string _name = grassBlock.Name + std::to_string(n);
                grassBlock.IsSelected = (current_index == itemIndex);
                if (ImGui::Selectable(_name.c_str(), grassBlock.IsSelected)) { current_index = itemIndex; }
                if (grassBlock.IsSelected) { grassBlock.ObjMenu(_name); }
                
                n++;
            }
        }
        ImGui::EndListBox();
    }
    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

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


#endif