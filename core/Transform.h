//
// Created by Gebruiker on 30-10-2026.
//

#ifndef RAWENGINE_TRANSFORM_H
#define RAWENGINE_TRANSFORM_H

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>

namespace core {
    class Transform {
    private:
        glm::mat4 modelMatrix;
        glm::mat4 scaleMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        glm::mat4 rotateMatrix;
        glm::mat4 translateMatrix = glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1);
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    public:
        Transform() : modelMatrix(1), rotateMatrix(1), scale(1) {}

        void Translate(glm::vec3 translation);
        void Rotate(glm::vec3 rotation);
        void Scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
        glm::vec3 GetFowDir() const;
        glm::vec3 GetPosition() const;
        glm::vec3 GetRotation() const;
        glm::vec3 GetScale() const;
    };
}


#endif //RAWENGINE_TRANSFORM_H
