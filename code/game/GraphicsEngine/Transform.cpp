#include "Transform.h"

namespace Graphics {

    Transform::Transform() {

    }

    void Transform::Rotate(glm::vec3 targetRotation) {
        rotation = targetRotation;
    }

    void Transform::Translate(glm::vec3 targetTranslation) {
        translation = targetTranslation;
    }

    void Transform::Scale(glm::vec3 targetScale) {
        scale = targetScale;
    }

    glm::mat4 Transform::GetModelMatrix() {
        glm::mat4 currentModel = glm::mat4_cast(rotation);
        currentModel = glm::translate(currentModel,translation);
        currentModel = glm::scale(currentModel, scale);
        return currentModel;
    }
}