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
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    
    // Reorder these to match constructor initialization order:
    int currentFrame;
    float frameCounter;
    float frameTime;
    Direction direction;
    float scale;
    bool isMoving;

    // Textures can stay in any order since they're initialized in constructor body
    Texture2D walkDown, walkUp, walkLeft, walkRight;
    Texture2D idleDown, idleUp, idleLeft, idleRight;
    Texture2D* currentSprite;
    int frameWidth, frameHeight;
    int maxFrames;

    Character(Vector2 startPos, const std::string& assetDir);
    ~Character();

    void Update(float deltaTime) override;
    void Draw() override;
    void DrawCollisionBox();

    bool CheckInWall();
};

#endif