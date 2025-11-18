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
private:
    static Character* player;
    Character();

public:

    bool dead = false;

    Vector2 position = {200.0, 200.0};
    Vector2 velocity = {0.0, 0.0};
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    
    // Reorder these to match constructor initialization order:
    int currentFrame = 0;
    float frameCounter = 0.0f;
    float frameTime = 0.1f;
    Direction direction = Direction::Down;
    float scale = 2.0f;
    bool isMoving = false;
    float moveSpeed = 250.0f;

    // Textures can stay in any order since they're initialized in constructor body
    Texture2D walkDown, walkUp, walkLeft, walkRight;
    Texture2D idleDown, idleUp, idleLeft, idleRight;
    Texture2D* currentSprite;
    int frameWidth, frameHeight;
    int maxFrames;
    Vector2 spriteOffset = {0.0, -20.0};

    float collisionWidth = 48;
    float collisionHeight = 32;

    ~Character();

    static Character* GetPlayer() {
        if (player == nullptr)
            player = new Character();
        return player;
    }

    void Update(float deltaTime) override;
    void Draw() override;

    void DrawCollisionBox();
    Rectangle GetCollisionBox();
    bool CheckInWall();

    void SetPosition(Vector2 newPosition) {
        position = newPosition;
    }
};



#endif