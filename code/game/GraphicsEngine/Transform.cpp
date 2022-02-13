#include "Transform.h"

namespace Graphics {

    Transform::Transform() {

    }

    void Transform::Rotate(glm::vec3) {

    }

    void Transform::Translate(glm::vec3) {

    }

    void Transform::Scale(glm::vec3) {

    }

    glm::mat4 Transform::GetModelMatrix() {
        glm::mat4 currentModel = glm::mat4(1.0f);
        //currentModel = glm::rotate(currentModel,)
        currentModel = glm::translate(currentModel,translation);
        currentModel = glm::scale(currentModel, scale);
        return currentModel;
    }
}