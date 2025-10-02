#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "raylib.h"
#include "raymath.h"

// Virtual class, functions have to be overridden by derived classes
class GameObject {
    public:
        virtual void Update(float deltaTime) = 0;
        virtual void Draw() = 0;
};

#endif