#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

typedef struct BasicObject {
    Vector2 position;
    Vector2 velocity;
    Color color;
} BasicObject;

BasicObject BasicObjectCreate(Vector2 position);
void BasicObjectUpdate(BasicObject *object, float deltaTime);
void BasicObjectDraw(BasicObject object);

#endif