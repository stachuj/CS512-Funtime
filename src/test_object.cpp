#include "test_object.hpp"
#include "tilemap.hpp"

void TestObject::Update(float deltaTime) {

    float speed = Vector2Distance(this->position, GetMousePosition());

    //we must multiply the speed by deltaTime or it will go too fast!!!
    Vector2 velocity = Vector2Subtract(Vector2MoveTowards(this->position, GetMousePosition(), speed * deltaTime), this->position);

    position.x += velocity.x;
    while (CheckInWall()) {
        position.x -= signf(velocity.x)/2;
    }

    position.y += velocity.y;
    while (CheckInWall()) {
        position.y -= signf(velocity.y)/2;
    }

}

void TestObject::Draw() {

    //draw the rectangle offset from the position so the x,y pos is at the center of the rectangle
    Rectangle rect = {this->position.x - 16, this->position.y - 16, 32, 32};

    //if the mouse is in the rectangle, body color is PINK, else it's PURPLE
    Color bodyColor = (CheckCollisionPointRec(GetMousePosition(), rect)) ? PINK : PURPLE;

    DrawRectangleRec(rect, bodyColor);
    DrawRectangleLinesEx(rect, 3, BLACK);
}

bool TestObject::CheckInWall() {
    int x = position.x - 16;
    int y = position.y - 16;
    int widthHeight = 32;
    return isWall(x, y) ||
            isWall(x+widthHeight, y) ||
            isWall(x, y+widthHeight) ||
            isWall(x+widthHeight, y+widthHeight);
}

float TestObject::signf(float x) {
    if (x > 0) return 1.0;
    if (x < 0) return -1.0;
    return 0.0;
}