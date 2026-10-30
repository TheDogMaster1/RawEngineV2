//
// Created by Gebruiker on 30-10-2026.
//

#ifndef RAWENGINE_TRANSFORM_H
#define RAWENGINE_TRANSFORM_H

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>

namespace Core {
    class Transform {
    private:
        glm::mat4 modelMatrix;
        glm::mat4 scaleMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        glm::mat4 rotateMatrix;
        glm::mat4 translateMatrix = glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1);
    public:

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 rotation);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
    };
}


#endif //RAWENGINE_TRANSFORM_H
