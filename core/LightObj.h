//
// Created by Gebruiker on 23-10-2026.
//

#ifndef RAWENGINE_LIGHTOBJ_H
#define RAWENGINE_LIGHTOBJ_H
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
    class LightObj {
    private:
        glm::vec4 color;
        glm::vec3 position;
        float ambientIntensity;
    public:
        LightObj(glm::vec3 color, glm::vec3 position, float ambientIntensity) : color(color, 1.0f), position(position), ambientIntensity(ambientIntensity) {};
        ~LightObj() = default;
        LightObj& operator=(const LightObj& anotherLight);
        void SetPos(glm::vec3 newPosition);
        void SetColor(glm::vec4 newColor);
        glm::vec4 GetColor() const;
        glm::vec3 GetPosition() const;
        float GetAmbientIntensity() const;
    };
}


#endif //RAWENGINE_LIGHTOBJ_H
