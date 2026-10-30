//
// Created by Gebruiker on 30-10-2026.
//

#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Core {

    void Transform::translate(const glm::vec3 translation) {
        translateMatrix = glm::translate(modelMatrix, translation);
    }

    void Transform::rotate(const glm::vec3 rotation) {
        rotateMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Transform::scale(const glm::vec3 scale) {
        scaleMatrix = glm::scale(modelMatrix, scale);
    }

    glm::mat4 Transform::getModelMatrix() const {
        return this->translateMatrix*rotateMatrix*scaleMatrix;
    }
}