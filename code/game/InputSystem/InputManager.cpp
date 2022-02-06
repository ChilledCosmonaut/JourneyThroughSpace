
   #include "InputManager.h"


namespace input{

     void InputManager::AddMouseCallback() {

    }

    void InputManager::AddScrollCallback() {

    }

    void InputManager::AddKeyboardCallback() {

    }

    void InputManager::CallMouseMovement(GLFWwindow *window, double xPosIn, double yPosIn) {
        for (auto & mouseEvent :mouseMoveEvents) {
                mouseEvent(window, xPosIn, yPosIn);
        }
    }

    void InputManager::CallScrollMovement(GLFWwindow *window, double xOffset, double yOffset) {
        for (auto & scrollEvent :scrollMoveEvent) {
            scrollEvent(window, xOffset, yOffset);
        }
    }

    void InputManager::StartListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, CallMouseMovement);
        glfwSetScrollCallback(window, CallScrollMovement);
    }
}