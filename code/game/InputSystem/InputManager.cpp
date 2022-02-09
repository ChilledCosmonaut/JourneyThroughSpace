#include "InputManager.h"

namespace input {
    /*template<typename inputFunction>
    int findFunctionInVector(inputFunction targetFunction, std::vector<inputFunction> functionVector) {
        auto res = find(functionVector.begin(), functionVector.end(), targetFunction);
        if (res != functionVector.end())
            return res - functionVector.begin();
        return -1;
    }*/

    /*template<typename inputFunction>
    void AddFunctionToVector(inputFunction targetFunction, std::vector<inputFunction> *functionVector) {
        //if (findFunctionInVector(targetFunction, functionVector) == NOT_FOUND)
        functionVector->push_back(targetFunction);
    }*/

    /*template<typename inputFunction>
    void RemoveFunctionToVector(inputFunction targetFunction, std::vector<inputFunction> functionVector) {
        int functionAtIndex = findFunctionInVector(targetFunction, functionVector);
        if (functionAtIndex == NOT_FOUND) return;
        functionVector.erase(std::next(functionVector.begin(), functionAtIndex));
    }*/

    [[maybe_unused]] void InputManager::StartListening(GLFWwindow *window) {
        glfwSetWindowUserPointer(window, this);
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
        glfwSetWindowUserPointer(window, nullptr);
    }

    [[maybe_unused]] void
    InputManager::AddMouseMoveCallback(const std::function<void(GLFWwindow *, double, double)>& mouseMoveCallback) {
        //AddFunctionToVector(mouseMoveCallback, mouseMoveEvents);
        mouseMoveEvents.push_back(mouseMoveCallback);
    }

    /*[[maybe_unused]] void
    InputManager::RemoveMouseMoveCallback(const std::function<void(GLFWwindow *, double, double)> &mouseMoveCallback) {
        //RemoveFunctionToVector(mouseMoveCallback, mouseMoveEvents);
    }*/

    [[maybe_unused]] void
    InputManager::AddScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {
        //AddFunctionToVector(scrollCallback, scrollMoveEvent);
        scrollMoveEvent.push_back(scrollCallback);
    }

    /*[[maybe_unused]] void
    InputManager::RemoveScrollCallback(const std::function<void(GLFWwindow *, double, double)> &scrollCallback) {
        //RemoveFunctionToVector(scrollCallback, scrollMoveEvent);
    }*/

    [[maybe_unused]] void
    InputManager::AddKeyboardCallback(const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        //AddFunctionToVector(keyInputCallback, keyInputEvent);
        keyInputEvent.push_back(keyInputCallback);
    }

    /*[[maybe_unused]] void InputManager::RemoveKeyboardCallback(
            const std::function<void(GLFWwindow *, int, int, int, int)> &keyInputCallback) {
        //RemoveFunctionToVector(keyInputCallback, keyInputEvent);
    }*/

    [[maybe_unused]] void
    InputManager::AddMouseButtonCallback(const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        //AddFunctionToVector(mouseButtonCallback, mouseButtonEvent);
        mouseButtonEvent.push_back(mouseButtonCallback);
    }

    /*[[maybe_unused]] void
    InputManager::RemoveMouseButtonCallback(
            const std::function<void(GLFWwindow *, int, int, int)> &mouseButtonCallback) {
        //RemoveFunctionToVector(mouseButtonCallback, mouseButtonEvent);
    }*/

    void InputManager::CallMouseMovement(GLFWwindow *window, double xPosIn, double yPosIn) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(window))->CallInstantiatedMouseMovements(window, xPosIn, yPosIn);
    }

    void InputManager::CallInstantiatedMouseMovements(GLFWwindow *window, double xPosIn, double yPosIn) {
        for (auto &mouseEvent: mouseMoveEvents) {
            mouseEvent(window, xPosIn, yPosIn);
        }
    }

    void InputManager::CallScrollMovement(GLFWwindow *window, double xOffset, double yOffset) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(window))->CallInstantiatedScrollMovement(window, xOffset, yOffset);
    }

    void InputManager::CallInstantiatedScrollMovement(GLFWwindow *window, double xOffset, double yOffset) {
        for (auto &scrollEvent: scrollMoveEvent) {
            scrollEvent(window, xOffset, yOffset);
        }
    }

    void InputManager::CallKeyMovement(GLFWwindow *window, int key, int scancode, int action, int mods) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(window))->CallInstantiatedKeyMovement(window, key, scancode, action, mods);
    }

    void InputManager::CallInstantiatedKeyMovement(GLFWwindow *window, int key, int scancode, int action, int mods) {
        for (auto &keyEvent: keyInputEvent) {
            keyEvent(window, key, scancode, action, mods);
        }
    }

    void InputManager::CallMouseButtonMovement(GLFWwindow *window, int button, int action, int mods) {
        static_cast<InputManager*>(glfwGetWindowUserPointer(window))->CallInstantiatedMouseButtonMovement(window, button, action, mods);
    }

    void InputManager::CallInstantiatedMouseButtonMovement(GLFWwindow *window, int button, int action, int mods) {
        for (auto &mouseEvent: mouseButtonEvent) {
            mouseEvent(window, button, action, mods);
        }
    }
}