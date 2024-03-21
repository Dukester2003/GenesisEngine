#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>
#include <array>

enum class KeyAction
{
    CAMERA_MOVE_FORWARD,    // W
    CAMERA_MOVE_BACKWARD,   // A
    CAMERA_MOVE_LEFT,       // S
    CAMERA_MOVE_RIGHT,      // D
    EXIT_APP,               // ESC
    INCREASE_SPEED,         // LEFT SHIFT 
    DECREASE_SPEED,         // LEFT SHIFT RELEASE
    // ... any other actions
};
class InputManager
{
public:
    // Map GLFW keys to actions
    std::map<int, KeyAction> keyToAction = {
        {GLFW_KEY_W, KeyAction::CAMERA_MOVE_FORWARD},
        {GLFW_KEY_S, KeyAction::CAMERA_MOVE_BACKWARD},
        {GLFW_KEY_A, KeyAction::CAMERA_MOVE_LEFT},
        {GLFW_KEY_D, KeyAction::CAMERA_MOVE_RIGHT},
        {GLFW_KEY_ESCAPE, KeyAction::EXIT_APP},
        {GLFW_KEY_LEFT_SHIFT, KeyAction::INCREASE_SPEED},
        // ... any other key-action mappings
    };


public:
    InputManager(GLFWwindow* window) : m_window(window) {}

    void Update() {
        // Update old states
        m_lastKeys = m_currentKeys;
        // Clear current state
        m_currentKeys.fill(false);

        // Update current key state
        for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
            if (glfwGetKey(m_window, i) == GLFW_PRESS) {
                m_currentKeys[i] = true;
            }
        }
    }

    // Returns if key is held down
    bool IsKeyPressed(int key) const {
        return m_currentKeys[key];
    }

    // Returns if key is released
    bool IsKeyReleased(int key) const {
        return !m_currentKeys[key] && m_lastKeys[key];
    }

    // Returns if key is pressed and only calls once unless key is released
    bool IsKeyJustPressed(int key) const {
        return m_currentKeys[key] && !m_lastKeys[key];
    }

private:
    GLFWwindow* m_window;
    std::array<bool, GLFW_KEY_LAST + 1> m_currentKeys{};
    std::array<bool, GLFW_KEY_LAST + 1> m_lastKeys{};
};

#endif // !INPUT_MANAGER_H