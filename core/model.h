#pragma once

#include "Transform.h"
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"


namespace core {
    class Model {
    private:
        std::vector<core::Mesh> meshes;
    public:
        Transform transform;
        Model(std::vector<core::Mesh> meshes) : meshes(meshes) {}
        void render(GLenum drawMode);

    };
}