#pragma once

#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

namespace input{
    class InputManager {
    public:
        static void StartListening(GLFWwindow* window);

        static void AddMouseCallback();
        static void AddScrollCallback();
        static void AddKeyboardCallback();

    private:
        static std::vector<std::function<void (GLFWwindow*, double, double)>> mouseMoveEvents;
        static std::vector<std::function<void (GLFWwindow*, double, double)>> scrollMoveEvent;
        static std::vector<std::function<void (GLFWwindow*, int, int, int, int)>> keyInputEvent;

        static void CallMouseMovement(GLFWwindow* window, double xPosIn, double yPosIn);
        static void CallScrollMovement(GLFWwindow* window, double xOffset, double yOffset);
        static void CallKeyMovement(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}