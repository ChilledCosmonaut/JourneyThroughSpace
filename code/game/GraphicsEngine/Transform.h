#pragma once

#include "glm/vec3.hpp"
#include "glm/gtx/dual_quaternion.hpp"

namespace Graphics {

    class Transform {
    public:
        explicit Transform(glm::vec3 startRotation = glm::vec3(0.0f,0.0f,0.0f),
                           glm::vec3 startTranslation = glm::vec3(0.0f,0.0f,0.0f),
                           glm::vec3 startScale = glm::vec3(1.0f,1.0f,1.0f));

        void Rotate(glm::vec3 targetRotation);
        void Translate(glm::vec3 targetTranslation);
        void Scale(glm::vec3 targetScale);

        glm::mat4* GetModelMatrix();
        glm::mat4 GetInverseModelMatrix();

    private:
        //glm::quat rotation{};
        glm::vec3 rotation{};
        //float rotationAngle = 0.0f;
        glm::vec3 translation{};
        glm::vec3 scale{};

        glm::mat4 modelMatrix{};
        glm::mat4 inverseModelMatrix{};

        void recalculateModel();
    };
}