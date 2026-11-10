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
        glm::mat4 baseMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 rotateMatrix;
        glm::mat4 translateMatrix;

    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform() : baseMatrix(1), translateMatrix(0), rotateMatrix(1), scaleMatrix(0), position(0), rotation(0), scale(1) {
            Translate(glm::vec3(0));
            Scale(glm::vec3(1));
            Rotate(glm::vec3(0));
        }

        void Translate(glm::vec3 translation);
        void Rotate(glm::vec3 rotation);
        void Scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
        glm::vec3 GetFowDir() const;
    };
}


#endif //RAWENGINE_TRANSFORM_H
