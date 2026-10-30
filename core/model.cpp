#include "model.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {
    void Model::render(GLenum drawMode) {
        for (int i = 0; i < meshes.size(); ++i) {
            meshes[i].render(drawMode);
        }
    }

    void Model::translate(glm::vec3 translation) {
        translateMatrix = glm::translate(modelMatrix, translation);
    }

    void Model::rotate(glm::vec3 rotation) {
        rotateMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void Model::scale(glm::vec3 scale) {
        scaleMatrix = glm::scale(modelMatrix, scale);
    }

    glm::mat4 Model::getModelMatrix() const {
        return this->translateMatrix*rotateMatrix*scaleMatrix;
    }
}