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
        static std::vector<std::function<void (GLFWwindow*, int, int, int, int)>> keyPressEvent;

        void CallMouseMovement(GLFWwindow* window, double xPosIn, double yPosIn);
        void CallScrollMovement(GLFWwindow* window, double xOffset, double yOffset);
    };
}