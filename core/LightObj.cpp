//
// Created by Gebruiker on 23-10-2026.
//

#include "LightObj.h"

namespace core {
    LightObj& LightObj::operator=(const LightObj& anotherLight) {
        if (this != &anotherLight) {
            this->color = anotherLight.color;
            this->type = anotherLight.type;
            this->specularIntensity = anotherLight.specularIntensity;
            this->transform = anotherLight.transform;
        }
        return *this;
    }
    void LightObj::SetPos(glm::vec3 newPosition) { transform.Translate(newPosition); }
    void LightObj::SetColor(glm::vec4 newColor) { this->color = newColor; }
    void LightObj::SetIntensity(float newIntensity) { this->intensity = newIntensity; }
    void LightObj::SetSpecularIntensity(float newSpecularIntensity) { this->specularIntensity = newSpecularIntensity; }
    void LightObj::SetType(float newType){ this->type = newType; }
    float LightObj::GetType() const { return this -> type; }
    glm::vec4 LightObj::GetColor() const { return  this -> color; }
    float LightObj::GetIntensity() const { return this -> intensity; }
    float LightObj::GetSpecularIntensity() const { return  this -> specularIntensity; }

}