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
ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse; // by ex
ImGuiWindowFlags sceneFlags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollWithMouse;
btVector3 _btPosition = btVector3(0.0f, 0.0f, 0.0f);
bool pushedBack;

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
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
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
            if (ImGui::MenuItem("Toggle Grid", "Ctrl+G"))
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
                // Process only newly added BoxColliders
                for (int i = lastProcessedIndex + 1; i < sphereColliders.size(); ++i) {
                    auto& sphereCollider = sphereColliders[i];
                    sphereCollider.massValue = 5.0f;
                    sphereCollider.InitiateRigidBody(dynamicsWorld);
                }

                lastProcessedIndex = sphereColliders.size() - 1;
                
            }
            if (ImGui::MenuItem("Cylinder"))
            {

            }
            if (ImGui::MenuItem("Capsule"))
            {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
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
        if (ImGui::TreeNode("Climbables"))
        {
            if (ImGui::Button("Bowser Flag"))
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
        for (int i = 0;auto& item : items)
        {
            i++;
            // Spawn Data for Player
            player->Name = "Player";
            player->IsSelected = (current_index == i);
            if (ImGui::Selectable(player->Name.c_str(), player->IsSelected)) { current_index = i; }
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
                i++;
                n++;
                box.Name = "Box";
                std::string _name = box.Name + std::to_string(n);
                box.IsSelected = (current_index == i);
                if (ImGui::Selectable(_name.c_str(), box.IsSelected))
                {
                    current_index = i;

                }
                if (box.IsSelected)
                {
                    box.ObjMenu(_name);

                }
            }

            for (int n = 0; auto & sphere : sphereColliders)
            {
                i++;
                n++;
                sphere.Name = "Sphere";
                std::string _name = sphere.Name + std::to_string(n);
                sphere.IsSelected = (current_index == i);
                if (ImGui::Selectable(_name.c_str(), sphere.IsSelected))
                {
                    current_index = i;

                }
                if (sphere.IsSelected)
                {
                    sphere.ObjMenu(_name);

                }
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
#endif