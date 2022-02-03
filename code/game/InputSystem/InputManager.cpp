#include "InputManager.h"


namespace input{

    void MouseMovement(GLFWwindow *window, double xPosIn, double yPosIn){

    }

    void InputManager::AddMouseCallback() {

    }

    void InputManager::AddScrollCallback() {

    }

    void InputManager::AddKeyboardCallback() {

    }

    void InputManager::CallMouseMovement(GLFWwindow *window, double xPosIn, double yPosIn) {

    }

    void InputManager::CallScrollMovement(GLFWwindow *window, double xOffset, double yOffset) {

    }

    void InputManager::StartListening(GLFWwindow *window) {
        auto callMouse = &this->CallMouseMovement;
        glfwSetCursorPosCallback(window, this->CallMouseMovement);
        glfwSetScrollCallback(window, scroll_callback);
    }
}