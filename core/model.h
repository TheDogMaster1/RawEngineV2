#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"

namespace core {
    class Model {
    private:
        std::vector<core::Mesh> meshes;
        glm::mat4 modelMatrix;
        glm::mat4 scaleMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        glm::mat4 rotateMatrix;
        glm::mat4 translateMatrix = glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1);
    public:
        Model(std::vector<core::Mesh> meshes) : meshes(meshes), modelMatrix(1), rotateMatrix(1), translateMatrix(1) {}

        void render(GLenum drawMode);

        void translate(glm::vec3 translation);
        void rotate(glm::vec3 axis, float radians);
        void scale(glm::vec3 scale);
        glm::mat4 getModelMatrix() const;
    };
}