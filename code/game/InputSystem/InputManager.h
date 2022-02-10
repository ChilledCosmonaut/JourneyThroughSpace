#pragma once

#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

namespace input{
    class [[maybe_unused]] InputManager {
    public:
        [[maybe_unused]] void StartListening(GLFWwindow* window);
        [[maybe_unused]] static void StopListening(GLFWwindow* window);

        std::vector<std::function<void (GLFWwindow*, double, double)>> mouseMoveEvents;
        std::vector<std::function<void (GLFWwindow*, double, double)>> scrollMoveEvent;
        std::vector<std::function<void (GLFWwindow*, int, int, int, int)>> keyInputEvent;
        std::vector<std::function<void (GLFWwindow*, int, int, int)>> mouseButtonEvent;


        [[maybe_unused]] void AddMouseMoveCallback(const std::function<void (GLFWwindow*, double, double)>& mouseMoveCallback);
        //[[maybe_unused]] static void RemoveMouseMoveCallback(const std::function<void(GLFWwindow *, double, double)> &mouseMoveCallback);

        [[maybe_unused]] void AddScrollCallback(const std::function<void (GLFWwindow*, double, double)>& scrollCallback);
        //[[maybe_unused]] static void RemoveScrollCallback(const std::function<void (GLFWwindow*, double, double)>& scrollCallback);

        [[maybe_unused]] void AddKeyboardCallback(const std::function<void (GLFWwindow*, int, int, int, int)>& keyInputCallback);
        //[[maybe_unused]] static void RemoveKeyboardCallback(const std::function<void (GLFWwindow*, int, int, int, int)>& keyInputCallback);

        [[maybe_unused]] void AddMouseButtonCallback(const std::function<void (GLFWwindow*, int, int, int)>& mouseButtonCallback);
        //[[maybe_unused]] static void RemoveMouseButtonCallback(const std::function<void (GLFWwindow*, int, int, int)>& mouseButtonCallback);

    private:

        static void CallMouseMovement(GLFWwindow* window, double xPosIn, double yPosIn);
        void CallInstantiatedMouseMovements(GLFWwindow* window, double xPosIn, double yPosIn);
        static void CallScrollMovement(GLFWwindow* window, double xOffset, double yOffset);
        void CallInstantiatedScrollMovement(GLFWwindow* window, double xOffset, double yOffset);
        static void CallKeyMovement(GLFWwindow* window, int key, int scancode, int action, int mods);
        void CallInstantiatedKeyMovement(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CallMouseButtonMovement(GLFWwindow* window, int button, int action, int mods);
        void CallInstantiatedMouseButtonMovement(GLFWwindow* window, int button, int action, int mods);
    };
}