//
// Created by Gebruiker on 30-10-2026.
//

#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>


namespace core {

    void Transform::Translate(const glm::vec3 translation) {
        this->position = translation;
        translateMatrix = glm::translate(baseMatrix, translation);
    }

    void Transform::Rotate(const glm::vec3 rotation) {
        this->rotation = rotation;
        rotateMatrix = glm::rotate(baseMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(baseMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(baseMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Transform::Scale(const glm::vec3 scale) {
        this->scale = scale;
        scaleMatrix = glm::scale(baseMatrix, scale);
    }

    glm::mat4 Transform::getModelMatrix() const {
        return this->translateMatrix*rotateMatrix*scaleMatrix;
    }
    glm::vec3 Transform::GetPosition() const {
        return this->position;
    }
    glm::vec3 Transform::GetFowDir() const {
        // return glm::vec3(rotateMatrix[0][2], rotateMatrix[1][2], rotateMatrix[2][2]);
        return glm::vec3(rotateMatrix[2]);
    }
    glm::vec3 Transform::GetRotation() const {
        return rotation;
    }
    glm::vec3 Transform::GetScale() const {
        return this->scale;
    }
}