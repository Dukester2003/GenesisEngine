#pragma once
#ifndef G_GUI_H
#define G_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Common_Assets.h"
#include "../CollisionShapes/InitiateCollision.h"

#include <dirent.h>
#include <sys/stat.h>
#include <filesystem> // Include this for std::filesystem::path

class GUI
{
public:
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse; // by ex
    ImGuiWindowFlags sceneFlags = ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollWithMouse;
    bool pushedBack;
    bool isPlaying = false;
    bool show_file_explorer;
    static bool showSaveWindow;
    static bool showLoadWindow;
    std::string current_path = "."; // Set an initial path, e.g., the current working directory
    bool isSceneHovered = false;
    static char filename[64];
public:
    GUI() {}
    void ImGuiSetup(GLFWwindow* window);
    void AddItem(std::shared_ptr<GameObject> item, btDynamicsWorld* dynamicsWorld);
    void ShowEngineGUI(Scene& scene);
    bool IsMouseHoveringOverWindow(const char* window_name);
    bool ListFiles(const std::string& path, std::vector<std::string>& files, std::vector<std::string>& directories);
    void FileExplorer(const std::string& title, std::string& current_path);
    void ShowSaveWindow(Scene& scene);
    void ShowLoadWindow(Scene& scene);

};

#endif