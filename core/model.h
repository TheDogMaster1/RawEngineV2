#pragma once

#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "mesh.h"
#include "Object.h"


namespace core {
    class Model : public Object {
    private:
        std::vector<core::Mesh> meshes;
    public:
        Model(std::vector<core::Mesh> meshes) : meshes(meshes) {}
        void render(GLenum drawMode);

    };
}
