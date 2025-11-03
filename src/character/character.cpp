#include "character.hpp"
#include "tilemap.hpp"

const std::string assetDir = "../../assets";
Character* Character::player = nullptr;

Character::Character() {
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

    if (dead)
        return;

    Vector2 oldPosition = position; // Store the old position
    
    velocity = {0, 0};
    isMoving = false;

    // Keybinds
    if (IsKeyDown(KEY_W)) { velocity.y = -1; direction = Direction::Up;    isMoving = true; }
    if (IsKeyDown(KEY_S)) { velocity.y =  1; direction = Direction::Down;  isMoving = true; }
    if (IsKeyDown(KEY_A)) { velocity.x = -1; direction = Direction::Left;  isMoving = true; }
    if (IsKeyDown(KEY_D)) { velocity.x =  1; direction = Direction::Right; isMoving = true; }

    if (Vector2Length(velocity) > 0)
        velocity = Vector2Normalize(velocity);

    float moveSpeed = 200.0f;
    Vector2 newPosition = Vector2Add(position, Vector2Scale(velocity, moveSpeed * deltaTime));

    // Try moving on X axis first
    position.x = newPosition.x;
    if (CheckInWall()) {
        position.x = oldPosition.x; // Revert X movement if collision
    }

    // Then try moving on Y axis
    position.y = newPosition.y;
    if (CheckInWall()) {
        position.y = oldPosition.y; // Revert Y movement if collision
    }

    // Update animation
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

bool Character::CheckInWall() {
    float halfWidth = collisionWidth / 2.0f; // bc of scaling the sprite
    float halfHeight = collisionHeight / 2.0f;
    
    // Check multiple points around the character's collision rectangle
    return isWall(position.x - halfWidth, position.y + halfHeight) ||  // bottom left
           isWall(position.x + halfWidth, position.y + halfHeight) ||  // bottom right
           isWall(position.x - halfWidth, position.y - halfHeight) ||  // top left  
           isWall(position.x + halfWidth, position.y - halfHeight); // top right
           //isWall(position.x, position.y + halfHeight) ||                            // bottom center
           //isWall(position.x, position.y - halfHeight) ||                            // top center
           //isWall(position.x - halfWidth * cornerSpread, position.y) ||              // middle left
           //isWall(position.x + halfWidth * cornerSpread, position.y);                // middle right
}


void Character::Draw() {

    if (dead)
        return;


    Rectangle src = {
        static_cast<float>(frameWidth * currentFrame),
        0,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    Rectangle dest = {
        position.x + spriteOffset.x,
        position.y + spriteOffset.y,
        frameWidth * scale,
        frameHeight * scale
    };

    Vector2 origin = { (frameWidth * scale) / 2.0f, (frameHeight * scale) / 2.0f };

    DrawTexturePro(*currentSprite, src, dest, origin, 0.0f, WHITE);

    // Draw collision box 
    //DrawCollisionBox(); //UNCOMMENT TO SEE COLLISION POINTS

    DrawCircle(position.x, position.y, 2, GREEN);
}

void Character::DrawCollisionBox() {
    
    float halfWidth = collisionWidth / 2.0f;
    float halfHeight = collisionHeight / 2.0f;
    
    DrawRectangleLines(position.x - halfWidth, position.y - halfHeight, collisionWidth, collisionHeight, RED);
    
}

Vector2 Character::GetPosition() const {
    return position;
}

Vector2 Character::GetSize() const {
    return { frameWidth * scale, frameHeight * scale };
}

Rectangle Character::GetCollisionBox() {

    float halfWidth = collisionWidth / 2.0f;
    float halfHeight = collisionHeight / 2.0f;

    return Rectangle{position.x - halfWidth, position.y - halfHeight, collisionWidth, collisionHeight};
}

