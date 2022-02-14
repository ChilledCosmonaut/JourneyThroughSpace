#include <iostream>
#include "Transform.h"

namespace Graphics {

    Transform::Transform(glm::vec3 startRotation, glm::vec3 startTranslation, glm::vec3 startScale) {
        translation = startTranslation;
        scale = startScale;
        Rotate(startRotation);
    }

    void Transform::Rotate(glm::vec3 targetRotation) {
        /*auto rotationVector = glm::vec3(0.0f);
        rotationVector.x = 1;
        rotationVector.y = targetRotation.y/targetRotation.x;
        rotationVector.z = targetRotation.z/targetRotation.x;
        rotationVector = glm::normalize(rotationVector);
        rotation = glm::quat(cos(glm::radians(targetRotation.x/2)),
                             rotationVector.x * sin(glm::radians(targetRotation.x/2)),
                             rotationVector.x * sin(glm::radians(targetRotation.x/2)),
                             rotationVector.x * sin(glm::radians(targetRotation.x/2)));*/
        /*rotationAxis = rotationVector;
        rotationAngle = rotationVector.x;*/
        rotation = glm::radians(targetRotation);
        recalculateModel();

    }

    void Transform::Translate(glm::vec3 targetTranslation) {
        translation = targetTranslation;
        recalculateModel();
    }

    void Transform::Scale(glm::vec3 targetScale) {
        scale = targetScale;
        recalculateModel();
    }

    glm::mat4* Transform::GetModelMatrix() {
        return &modelMatrix;
    }

    glm::mat4 Transform::GetInverseModelMatrix() {
        return inverseModelMatrix;
    }

    void Transform::recalculateModel() {
        /*glm::mat4 test = glm::mat4 (1.0f);
        test = glm::translate(test, translation);
        test = glm::rotate(test, rotationAngle, rotationAxis);
        test = glm::scale(test, scale);
        modelMatrix = test;*/
        glm::mat4 translateModel = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 rotateModel = glm::mat4_cast(glm::quat(rotation));
        glm::mat4 scaleModel = glm::scale(glm::mat4(1.0f), scale);
        modelMatrix = translateModel * rotateModel * scaleModel;
        inverseModelMatrix = scaleModel * rotateModel * translateModel;
    }
}