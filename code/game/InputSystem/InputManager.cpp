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

    template<typename function>
    void AddFunctionToVector(function targetFunction, std::vector<function> functionVector) {
        if (findFunctionInVector(targetFunction, functionVector) == NOT_FOUND)
            functionVector.push_back(targetFunction);
    }

    template<typename function>
    void RemoveFunctionToVector(function targetFunction, std::vector<function> functionVector) {
        int functionAtIndex = findFunctionInVector(targetFunction, functionVector);
        if (functionAtIndex == NOT_FOUND) return;
        functionVector.erase(std::next(functionVector.begin(), functionAtIndex));
    }

    void InputManager::RemoveScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {

    }

    [[maybe_unused]] void
    InputManager::AddKeyboardCallback(const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        if (findFunctionInVector(keyInputCallback, keyInputEvent) == NOT_FOUND)
            keyInputEvent.push_back(keyInputCallback);
    }

    void InputManager::RemoveKeyboardCallback(
            const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {

    }

    [[maybe_unused]] void
    InputManager::AddMouseButtonCallback(const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        if (findFunctionInVector(mouseButtonCallback, mouseButtonEvent) == NOT_FOUND)
            mouseButtonEvent.push_back(mouseButtonCallback);
    }

    void
    InputManager::RemoveMouseButtonCallback(const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {

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