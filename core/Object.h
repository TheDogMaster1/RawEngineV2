//
// Created by Gebruiker on 20-11-2026.
//

#ifndef RAWENGINE_OBJECT_H
#define RAWENGINE_OBJECT_H
#include <string>
#include "Transform.h"

namespace core {
    class Object {
    protected:
        std::string name = "obj";
    public:
        Transform transform;
        void SetName(std::string name);
    };
} // core

#endif //RAWENGINE_OBJECT_H
