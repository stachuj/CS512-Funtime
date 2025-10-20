#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "game_object.hpp"
#include <string>

enum class Direction {
    Down,
    Up,
    Left,
    Right
};

class Character : public GameObject {
public:
    Vector2 position;
    Vector2 velocity;


    Texture2D walkDown, walkUp, walkLeft, walkRight;

    Texture2D idleDown, idleUp, idleLeft, idleRight;

    Texture2D* currentSprite;

    Direction direction;
    bool isMoving;

    int frameWidth, frameHeight;
    int currentFrame, maxFrames;
    float frameTime, frameCounter;

    float scale;

    Character(Vector2 startPos, const std::string& assetDir);
    ~Character();

    void Update(float deltaTime) override;
    void Draw() override;
    void DrawCollisionBox();

    bool CheckInWall();
};

#endif
