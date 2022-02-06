
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
        glfwSetKeyCallback(window, CallKeyMovement);
        glfwSetMouseButtonCallback(window, CallMouseButtonMovement);
    }

    void InputManager::CallKeyMovement(GLFWwindow *window, int key, int scancode, int action, int mods) {
        for (auto & keyEvent :keyInputEvent) {
            keyEvent(window, key, scancode, action, mods);
        }
    }

    void InputManager::CallMouseButtonMovement(GLFWwindow *window, int button, int action, int mods) {
        for (auto & mouseEvent :mouseButtonEvent) {
            mouseEvent(window, button, action, mods);
        }
    }

    void InputManager::StopListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
        glfwSetKeyCallback(window, nullptr);
        glfwSetMouseButtonCallback(window, nullptr);
    }
}