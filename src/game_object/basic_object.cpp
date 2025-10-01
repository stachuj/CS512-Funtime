#include "basic_object.hpp"

BasicObject BasicObjectCreate(Vector2 position) {

    return {
        .position = position,
        .velocity = {12.0, 0.0},
        .color = PURPLE
    };

}

void BasicObjectUpdate(BasicObject *object, float deltaTime) {

    object->position = Vector2Add(object->position, Vector2Scale(object->velocity, deltaTime));

}

void BasicObjectDraw(BasicObject object) {

    int x = object.position.x;
    int y = object.position.y;

    DrawRectangle(x,y,64,64,object.color);

}
