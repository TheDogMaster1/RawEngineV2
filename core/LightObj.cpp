//
// Created by Gebruiker on 23-10-2026.
//

#include "LightObj.h"

namespace core {
    LightObj& LightObj::operator=(const LightObj& anotherLight) {
        if (this != &anotherLight) {
            this->color = anotherLight.color;
            this->transform = anotherLight.transform;
        }
        return *this;
    }
    void LightObj::SetPos(glm::vec3 newPosition) { transform.Translate(newPosition); }
    void LightObj::SetColor(glm::vec4 newColor) { this->color = newColor; }
    void LightObj::SetType(float newType){ this->type = newType; }
    float LightObj::GetType() const { return this -> type; }
    glm::vec4 LightObj::GetColor() const { return  this -> color; }

}