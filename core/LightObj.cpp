//
// Created by Gebruiker on 23-10-2026.
//

#include "LightObj.h"

namespace Core {
    LightObj& LightObj::operator=(const LightObj& anotherLight) {
        if (this != &anotherLight) {
            this->color = anotherLight.color;
            this->position = anotherLight.position;
        }
        return *this;
    }
    void LightObj::SetPos(glm::vec3 newPosition) { this->position = newPosition; }
    void LightObj::SetColor(glm::vec4 newColor) { this->color = newColor; }
    glm::vec4 LightObj::GetColor() const { return  this -> color; }
    glm::vec3 LightObj::GetPosition() const { return this -> position; }
}