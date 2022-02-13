#pragma once

#include "glm/vec3.hpp"
#include "glm/gtx/dual_quaternion.hpp"

namespace Graphics {

    class Transform {
    public:
        Transform();

        void Rotate(glm::vec3);

        void Translate(glm::vec3);

        void Scale(glm::vec3);

        glm::mat4 GetModelMatrix();

    private:
        glm::quat quaternion{};
        glm::mat4 rotation;
        glm::vec3 translation;
        glm::vec3 scale;
    };
}