#include "InputManager.h"

#define NOT_FOUND -1

namespace input {

    template<typename inputScript, typename function>
    struct inputFunction{
        inputScript* memberClass;
        std::function<function> memberFunction;
    };

    template<typename inputFunction>
    int findFunctionInVector(inputFunction targetFunction, std::vector<inputFunction> functionVector) {
        auto res = find(functionVector.begin(), functionVector.end(), targetFunction);
        if (res != functionVector.end())
            return res - functionVector.begin();
        return -1;
    }

    template<typename inputFunction>
    void AddFunctionToVector(inputFunction targetFunction, std::vector<inputFunction> functionVector) {
        if (findFunctionInVector(targetFunction, functionVector) == NOT_FOUND)
            functionVector.push_back(targetFunction);
    }

    template<typename inputFunction>
    void RemoveFunctionToVector(inputFunction targetFunction, std::vector<inputFunction> functionVector) {
        int functionAtIndex = findFunctionInVector(targetFunction, functionVector);
        if (functionAtIndex == NOT_FOUND) return;
        functionVector.erase(std::next(functionVector.begin(), functionAtIndex));
    }

    [[maybe_unused]] void InputManager::StartListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, CallMouseMovement);
        glfwSetScrollCallback(window, CallScrollMovement);
        glfwSetKeyCallback(window, CallKeyMovement);
        glfwSetMouseButtonCallback(window, CallMouseButtonMovement);
    }

    [[maybe_unused]] void InputManager::StopListening(GLFWwindow *window) {
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
        glfwSetKeyCallback(window, nullptr);
        glfwSetMouseButtonCallback(window, nullptr);
    }

    [[maybe_unused]] void
    InputManager::AddMouseMoveCallback(const std::function<void(GLFWwindow *, double, double)> &mouseMoveCallback) {
        AddFunctionToVector(mouseMoveCallback, mouseMoveEvents);
    }

    [[maybe_unused]] void
    InputManager::RemoveMouseMoveCallback(const std::function<void(GLFWwindow *, double, double)> &mouseMoveCallback) {
        RemoveFunctionToVector(mouseMoveCallback, mouseMoveEvents);
    }

    [[maybe_unused]] void
    InputManager::AddScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {
        AddFunctionToVector(scrollCallback, scrollMoveEvent);
    }

    [[maybe_unused]] void
    InputManager::RemoveScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {
        RemoveFunctionToVector(scrollCallback, scrollMoveEvent);
    }

    [[maybe_unused]] void
    InputManager::AddKeyboardCallback(const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        AddFunctionToVector(keyInputCallback, keyInputEvent);
    }

    [[maybe_unused]] void InputManager::RemoveKeyboardCallback(
            const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        RemoveFunctionToVector(keyInputCallback, keyInputEvent);
    }

    [[maybe_unused]] void
    InputManager::AddMouseButtonCallback(const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        AddFunctionToVector(mouseButtonCallback, mouseButtonEvent);
    }

    [[maybe_unused]] void
    InputManager::RemoveMouseButtonCallback(
            const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        RemoveFunctionToVector(mouseButtonCallback, mouseButtonEvent);
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
}