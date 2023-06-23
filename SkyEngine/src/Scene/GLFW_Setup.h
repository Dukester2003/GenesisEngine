#pragma once
#ifndef GLFW_SETUP
#define GLFW_SETUP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GUI.h"
class GLFW_Setup {
public:
    Scene* scene;
    GLFWwindow* window;
    const unsigned int SCR_WIDTH = 1200;
    const unsigned int SCR_HEIGHT = 900;
public:
    void Initiate()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }
    GLFW_Setup() {
        Initiate();
        // ...and the window
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this); // Store the `this` pointer in the GLFWwindow
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

        // configure global opengl state	
        // -----------------------------	
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glEnable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        // Retrieve the `this` pointer from the GLFWwindow
        GLFW_Setup* instance = static_cast<GLFW_Setup*>(glfwGetWindowUserPointer(window));

        // Use the `this` pointer to call the non-static member function
        instance->actual_framebuffer_size_callback(window, width, height);
    }

    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        GLFW_Setup* instance = static_cast<GLFW_Setup*>(glfwGetWindowUserPointer(window));
        instance->actual_mouse_callback(window, xposIn, yposIn);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        GLFW_Setup* instance = static_cast<GLFW_Setup*>(glfwGetWindowUserPointer(window));
        instance->actual_scroll_callback(window, xoffset, yoffset);
    }

    // The actual callback implementations
    void actual_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    void actual_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);


        POINT cursorPos;
        GetCursorPos(&cursorPos);
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        {
            scene->camera.ProcessMouseMovement(xoffset, yoffset);
        }
    }

    void actual_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        scene->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
private:
    
    // settings
    
    // camera

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
};




#endif // !GLFW_SETUP




