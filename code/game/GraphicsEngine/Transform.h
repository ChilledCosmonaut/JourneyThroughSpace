#pragma once

#include "glm/vec3.hpp"
#include "glm/gtx/dual_quaternion.hpp"

namespace Graphics {

    class Transform {
    public:
        Transform();

        void Rotate(glm::vec3 targetRotation);
        void Translate(glm::vec3 targetTranslation);
        void Scale(glm::vec3 targetScale);

        glm::mat4 GetModelMatrix();

    private:
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 scale;
    };
}