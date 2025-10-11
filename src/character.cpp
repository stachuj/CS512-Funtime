#include "character.hpp"

Character::Character(Vector2 startPos, const std::string& assetDir)
    : position(startPos), velocity({0, 0}), currentFrame(0),
      frameCounter(0.0f), frameTime(0.15f), direction(Direction::Down),
      scale(2.0f), isMoving(false)
{
    // Walking pictures
    walkDown = LoadTexture((assetDir + "/run_down.png").c_str());
    walkUp   = LoadTexture((assetDir + "/run_up.png").c_str());
    walkLeft = LoadTexture((assetDir + "/run_left.png").c_str());
    walkRight= LoadTexture((assetDir + "/run_right.png").c_str());

    // idle pictures
    idleDown = LoadTexture((assetDir + "/idle_down.png").c_str());
    idleUp   = LoadTexture((assetDir + "/idle_up.png").c_str());
    idleLeft = LoadTexture((assetDir + "/idle_left.png").c_str());
    idleRight= LoadTexture((assetDir + "/idle_right.png").c_str());

    currentSprite = &idleDown; // start facing down

    // amt of frames on the sprite sheets
    maxFrames = 8;
    frameWidth  = walkDown.width / maxFrames;
    frameHeight = walkDown.height;
}

Character::~Character() {
    UnloadTexture(walkDown);
    UnloadTexture(walkUp);
    UnloadTexture(walkLeft);
    UnloadTexture(walkRight);
    UnloadTexture(idleDown);
    UnloadTexture(idleUp);
    UnloadTexture(idleLeft);
    UnloadTexture(idleRight);
}

void Character::Update(float deltaTime) {
    velocity = {0, 0};
    isMoving = false;

    //Keybinds, probably should change to have keybinds elsewhere once we have more controls
    if (IsKeyDown(KEY_W)) { velocity.y = -1; direction = Direction::Up;    isMoving = true; }
    if (IsKeyDown(KEY_S)) { velocity.y =  1; direction = Direction::Down;  isMoving = true; }
    if (IsKeyDown(KEY_A)) { velocity.x = -1; direction = Direction::Left;  isMoving = true; }
    if (IsKeyDown(KEY_D)) { velocity.x =  1; direction = Direction::Right; isMoving = true; }

    if (Vector2Length(velocity) > 0)
        velocity = Vector2Normalize(velocity);

    float moveSpeed = 200.0f;
    position = Vector2Add(position, Vector2Scale(velocity, moveSpeed * deltaTime));

    // correct sprite sheets
    switch (direction) {
        case Direction::Up:    currentSprite = isMoving ? &walkUp   : &idleUp;   break;
        case Direction::Down:  currentSprite = isMoving ? &walkDown : &idleDown; break;
        case Direction::Left:  currentSprite = isMoving ? &walkLeft : &idleLeft; break;
        case Direction::Right: currentSprite = isMoving ? &walkRight: &idleRight;break;
    }

    // Update frames
    frameCounter += deltaTime;
    if (frameCounter >= frameTime) {
        frameCounter = 0.0f;
        currentFrame = (currentFrame + 1) % maxFrames;
    }
}

void Character::Draw() {
    Rectangle src = {
        static_cast<float>(frameWidth * currentFrame),
        0,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    Rectangle dest = {
        position.x,
        position.y,
        frameWidth * scale,
        frameHeight * scale
    };

    Vector2 origin = { (frameWidth * scale) / 2.0f, (frameHeight * scale) / 2.0f };

    DrawTexturePro(*currentSprite, src, dest, origin, 0.0f, WHITE);
}
