#include "test_object_A_Star.hpp"


void TestObjectAStar::Update(float deltaTime) {

    Character* playerPtr = Character::GetPlayer();

    if(playerPtr->dead)
        return;

    if(checkOnTile()){
        setNextTile();
    }


    if (!awake) {
        if (Vector2Distance(playerPtr->position, position) < 120.0) {
            awake = true;
            pathingTimer = 0.50;
        }
    }
    else {
        // Update AStar for testEnemy every 3 seconds
        pathingTimer -= deltaTime ;
        
        if(pathingTimer <= 0.0) {
            targetPosition = {Character::GetPlayer()->position.x, Character::GetPlayer()->position.y};
            pathingTimer = 1.0/60.0;
            int testEnemyColTile = getTilePos(position.x) ;
            int testEnemyRowTile = getTilePos(position.y) ;
            path = AStarSearch(testEnemyRowTile, testEnemyColTile, getTilePos(targetPosition.x), getTilePos(targetPosition.y)) ;
        }
    }

    Vector2 destinationPosition = {nextTile.second * TILE_SIZE + TILE_SIZE/2, nextTile.first * TILE_SIZE + TILE_SIZE/2};

    velocity = Vector2Subtract(Vector2MoveTowards(position, destinationPosition, moveSpeed * deltaTime), position);

    position.x += velocity.x;
    while (CheckInWall()) {
        position.x -= signf(velocity.x)/2;
    }

    position.y += velocity.y;
    while (CheckInWall()) {
        position.y -= signf(velocity.y)/2;
    }

    Rectangle collisionBox = {position.x - collisionWidth/2.0, position.y - collisionHeight/2.0, collisionWidth, collisionHeight};

    if (CheckCollisionRecs(playerPtr->GetCollisionBox(), collisionBox)) {
        playerPtr->dead = true;
    }

    // --- animation update ---
    // choose row based on velocity direction (only update row when moving)
    updateAnimationDirection();

    // advance frame if moving (if no movement, you can keep a default idle frame)
    if (fabsf(velocity.x) > 0.0001f || fabsf(velocity.y) > 0.0001f) {
        animTimer += deltaTime;
        if (animTimer >= frameTime) {
            animTimer = fmodf(animTimer, frameTime);
            currentFrame = (currentFrame + 1) % framesPerRow;
        }
    } else {
        // idle: optionally set to middle frame (1) to avoid harsh 0-frame idle
        currentFrame = 1 % framesPerRow;
        animTimer = 0.0f;
    }
}

void TestObjectAStar::setPath(std::stack<Pair> newPath) {

    path = newPath ;
    // previousTile = std::make_pair(-1, -1) ;
    setNextTile() ;
    return ;
}

void TestObjectAStar::setNextTile() {

    if(!path.empty()) {
        // previousTile = nextTile ;
        nextTile = path.top() ;
        path.pop() ;
    }

    return ;
}

bool TestObjectAStar::checkOnTile() {

    Vector2 destinationPosition = {nextTile.second * TILE_SIZE + TILE_SIZE/2, nextTile.first * TILE_SIZE + TILE_SIZE/2};

    return Vector2Distance(position, destinationPosition) < 1.0;
}

void TestObjectAStar::Draw() {

    //draw the rectangle offset from the position so the x,y pos is at the center of the rectangle
    Rectangle rect = {position.x - collisionWidth/2.0, position.y - collisionHeight/2.0, collisionWidth, collisionHeight};

    //if the mouse is in the rectangle, body color is PINK, else it's PURPLE
    Color bodyColor = (awake) ? RED : GRAY;
    Color noticeColor = (fmodf(pathingTimer, 0.1f) < 0.05f) ? ORANGE : BLUE;

    // draw "!" as before
    if (pathingTimer > 0.1) {
        DrawText("!", position.x-1, position.y-48, 32, noticeColor);
    }

    if (spriteSheet.id != 0 && frameSize.x > 0 && frameSize.y > 0) {

    Rectangle src = {
        currentFrame * frameSize.x,
        currentRow * frameSize.y,
        frameSize.x,
        frameSize.y
    };

    // scaled width/height
    float scaledW = frameSize.x * spriteScale;
    float scaledH = frameSize.y * spriteScale;

    Rectangle dest = {
        position.x,
        position.y,
        scaledW,
        scaledH
    };

    Vector2 origin = { scaledW / 2.0f, scaledH / 2.0f };

    DrawTexturePro(
        spriteSheet,
        src,      // sub-rectangle from spritesheet
        dest,     // where to draw (scaled)
        origin,   // center
        0.0f,     // rotation
        WHITE
    );
}
}


// private helper: set currentRow based on velocity
void TestObjectAStar::updateAnimationDirection() {
    // Default behavior: if horizontal movement dominates, choose left/right, otherwise up/down.
    float ax = fabsf(velocity.x);
    float ay = fabsf(velocity.y);

    if (ax > ay + 0.001f) {
        // moving horizontally
        if (velocity.x > 0.0f) currentRow = 1; // right row
        else currentRow = 3;                  // left row
    } else if (ay > ax + 0.001f) {
        // moving vertically
        // note: y-axis sign depends on coordinate system. In raylib, +y goes down.
        if (velocity.y < 0.0f) currentRow = 0; // up row (y decreasing)
        else currentRow = 2;                  // down row (y increasing)
    }
    // if not moving much, keep currentRow as-is (idle shows middle frame)
}
