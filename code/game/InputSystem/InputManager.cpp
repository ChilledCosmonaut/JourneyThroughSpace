#include "InputManager.h"
#define NOT_FOUND -1

namespace input {

    template<typename function>
    int findFunctionInVector(function targetFunction, std::vector<function> functionVector) {
        auto res = find(functionVector.begin(), functionVector.end(), targetFunction);
        if (res != functionVector.end())
            return res - functionVector.begin();
        return -1;
    }

    [[maybe_unused]] void InputManager::AddMouseCallback(const std::function<void(GLFWwindow *, double, double)> &mouseMoveCallback) {
        if (findFunctionInVector(mouseMoveCallback, mouseMoveEvents) == NOT_FOUND)
            mouseMoveEvents.push_back(mouseMoveCallback);
    }

    [[maybe_unused]] void InputManager::AddScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {
        if (findFunctionInVector(scrollCallback, scrollMoveEvent) == NOT_FOUND)
            scrollMoveEvent.push_back(scrollCallback);
    }

    [[maybe_unused]] void
    InputManager::AddKeyboardCallback(const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        if (findFunctionInVector(keyInputCallback, keyInputEvent) == NOT_FOUND)
            keyInputEvent.push_back(keyInputCallback);
    }

    [[maybe_unused]] void
    InputManager::AddKeyboardCallback(const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        if (findFunctionInVector(mouseButtonCallback, mouseButtonEvent) == NOT_FOUND)
            mouseButtonEvent.push_back(mouseButtonCallback);
    }

    void InputManager::CallMouseMovement(GLFWwindow *window, double xPosIn, double yPosIn) {
        for (auto &mouseEvent: mouseMoveEvents) {
            mouseEvent(window, xPosIn, yPosIn);
        }
    }

    void InputManager::CallScrollMovement(GLFWwindow *window, double xOffset, double yOffset) {
        for (auto &scrollEvent: scrollMoveEvent) {
            scrollEvent(window, xOffset, yOffset);
        }
    }

    [[maybe_unused]] void InputManager::StartListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, CallMouseMovement);
        glfwSetScrollCallback(window, CallScrollMovement);
        glfwSetKeyCallback(window, CallKeyMovement);
        glfwSetMouseButtonCallback(window, CallMouseButtonMovement);
    }

    void InputManager::CallKeyMovement(GLFWwindow *window, int key, int scancode, int action, int mods) {
        for (auto &keyEvent: keyInputEvent) {
            keyEvent(window, key, scancode, action, mods);
        }
    }

    void InputManager::CallMouseButtonMovement(GLFWwindow *window, int button, int action, int mods) {
        for (auto &mouseEvent: mouseButtonEvent) {
            mouseEvent(window, button, action, mods);
        }
    }

    [[maybe_unused]] void InputManager::StopListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
        glfwSetKeyCallback(window, nullptr);
        glfwSetMouseButtonCallback(window, nullptr);
    }
}