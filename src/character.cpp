#include "character.hpp"
#include "tilemap/tilemap.hpp"

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
    // Get the actual dimensions of your character's collision area
    float collisionWidth = frameWidth * scale * 0.6f;   // 60% of sprite width
    float collisionHeight = frameHeight * scale * 0.4f; // 40% of sprite height (lower part)
    
    float halfWidth = collisionWidth / 2.0f; // bc of scaling the sprite
    float halfHeight = collisionHeight / 2.0f;
    
    float cornerSpread = 0.3f; //to move corners left and right
    
    // Check multiple points around the character's collision rectangle
    return isWall(position.x - halfWidth * cornerSpread, position.y + halfHeight) ||  // bottom left
           isWall(position.x + halfWidth * cornerSpread, position.y + halfHeight) ||  // bottom right
           isWall(position.x - halfWidth * cornerSpread, position.y - halfHeight) ||  // top left  
           isWall(position.x + halfWidth * cornerSpread, position.y - halfHeight) ||  // top right
           isWall(position.x, position.y + halfHeight) ||                            // bottom center
           isWall(position.x, position.y - halfHeight) ||                            // top center
           isWall(position.x - halfWidth * cornerSpread, position.y) ||              // middle left
           isWall(position.x + halfWidth * cornerSpread, position.y);                // middle right
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

    // Draw collision box 
    // DrawCollisionBox(); //UNCOMMENT TO SEE COLLISION POINTS
}

void Character::DrawCollisionBox() {
    float collisionWidth = frameWidth * scale * 0.6f;
    float collisionHeight = frameHeight * scale * 0.4f;
    
    float halfWidth = collisionWidth / 2.0f;
    float halfHeight = collisionHeight / 2.0f;
    
    float cornerSpread = 0.3f; 
    
    float pointSize = 4.0f;
    
    // Corner points 
    DrawCircle(position.x - halfWidth * cornerSpread, position.y + halfHeight, pointSize, RED);  // bottom left
    DrawCircle(position.x + halfWidth * cornerSpread, position.y + halfHeight, pointSize, RED);  // bottom right
    DrawCircle(position.x - halfWidth * cornerSpread, position.y - halfHeight, pointSize, RED);  // top left  
    DrawCircle(position.x + halfWidth * cornerSpread, position.y - halfHeight, pointSize, RED);  // top right
    DrawCircle(position.x - halfWidth * cornerSpread, position.y, pointSize, RED);            // middle left
    DrawCircle(position.x + halfWidth * cornerSpread, position.y, pointSize, RED);                // middle right
    
    //Middle points 
    DrawCircle(position.x, position.y + halfHeight, pointSize, RED);  // bottom center
    DrawCircle(position.x, position.y - halfHeight, pointSize, RED);  // top center
    
}

Vector2 Character::GetPosition() const {
    return position;
}

Vector2 Character::GetSize() const {
    return { frameWidth * scale, frameHeight * scale };
}
