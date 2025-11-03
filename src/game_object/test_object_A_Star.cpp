#include "test_object_A_Star.hpp"

void TestObjectAStar::Update(float deltaTime) {

    Character* playerPtr = Character::GetPlayer();

    if(playerPtr->dead)
        return;

    if(checkOnTile()){
        setNextTile();
    }


    if (!awake) {
        if (Vector2Distance(playerPtr->position, position) < 80.0) {
            awake = true;
            pathingTimer = 0.75;
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

    //displayPath(path);
    if (!awake)
        DrawCircleLines(position.x, position.y, 80.8, ORANGE);

    if (pathingTimer > 0.1) {
        DrawText("!", position.x, position.y-48, 32, noticeColor);
    }

    DrawRectangleRec(rect, bodyColor);
    DrawRectangleLinesEx(rect, 3, BLACK);
}