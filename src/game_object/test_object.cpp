#include "test_object.hpp"

void TestObject::Update(float deltaTime) {

    float speed = Vector2Distance(this->position, GetMousePosition());

    //we must multiply the speed by deltaTime or it will go too fast!!!
    this->position = Vector2MoveTowards(this->position, GetMousePosition(), speed * deltaTime);

}

void TestObject::Draw() {

    //draw the rectangle offset from the position so the x,y pos is at the center of the rectangle
    Rectangle rect = {this->position.x - 16, this->position.y - 16, 32, 32};

    //if the mouse is in the rectangle, body color is PINK, else it's PURPLE
    Color bodyColor = (CheckCollisionPointRec(GetMousePosition(), rect)) ? PINK : PURPLE;

    DrawRectangleRec(rect, bodyColor);
    DrawRectangleLinesEx(rect, 3, BLACK);
}