//
// Created by Gebruiker on 23-10-2026.
//

#ifndef RAWENGINE_LIGHTOBJ_H
#define RAWENGINE_LIGHTOBJ_H
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

namespace core {
    class LightObj {
    private:
        glm::vec4 color;
        float type;
    public:
        Transform transform;
        LightObj(glm::vec3 color, glm::vec3 position, float type) : color(color, 1.0f), type(type) {
            transform.position = position;
        }
        LightObj() {
            color = glm::vec4(1.0f);
            type = 0.0f;
        }
        ~LightObj() = default;
        LightObj& operator=(const LightObj& anotherLight);
        void SetPos(glm::vec3 newPosition);
        void SetColor(glm::vec4 newColor);
        void SetType(float newType);
        float GetType() const;
        glm::vec4 GetColor() const;
    };
}


#endif //RAWENGINE_LIGHTOBJ_H
