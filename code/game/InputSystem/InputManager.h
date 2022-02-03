#pragma once

#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

namespace input{
    class InputManager {
    public:
        void StartListening(GLFWwindow* window);

        void AddMouseCallback();
        void AddScrollCallback();
        void AddKeyboardCallback();

    private:
        std::vector<std::function<void (GLFWwindow*, double, double)>> mouseMovement;
        std::vector<std::function<void (GLFWwindow*, double, double)>> scrollMovement;
        std::vector<std::function<void (GLFWwindow*)>> keyboardInput;

        void CallMouseMovement(GLFWwindow* window, double xPosIn, double yPosIn);
        void CallScrollMovement(GLFWwindow* window, double xOffset, double yOffset);
    };
}