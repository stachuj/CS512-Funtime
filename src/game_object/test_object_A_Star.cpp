#include "test_object_A_Star.hpp"

void TestObjectAStar::Update(float deltaTime) {

    if(checkOnTile() || previousTile.first == -1){
        setNextTile() ;
    }

    float speed = 30.0 ;

    float xVelocity = speed * xDirection * deltaTime ;

    float yVelocity = speed * yDirection * deltaTime ;

    position.x += xVelocity;
    while (CheckInWall()) {
        position.x -= signf(xVelocity)/2;
    }

    position.y += yVelocity;
    while (CheckInWall()) {
        position.y -= signf(yVelocity)/2;
    }
    
}

void TestObjectAStar::setPath(std::stack<Pair> newPath) {

    path = newPath ;
    return ;
}

void TestObjectAStar::setNextTile() {

    if(!path.empty()) {
        previousTile = nextTile ;
        nextTile = path.top() ;
        path.pop() ;
    }
    else {
        nextTile = previousTile ;
    }
    setDirection() ;
    return ;
}

bool TestObjectAStar::checkOnTile() {
    int x = position.x - 16 ;
    int y = position.y - 16 ;
    int widthHeight = 32 ;
    return (getTilePos(x) == nextTile.second &&
            getTilePos(x + widthHeight) == nextTile.second && 
            getTilePos(y) == nextTile.first && 
            getTilePos(y + widthHeight) == nextTile.first) ;
}

void TestObjectAStar::setDirection() {
    if(previousTile.first > nextTile.first) {
        yDirection = -1 ;
    }
    else if(previousTile.first < nextTile.first) {
        yDirection = 1 ;
    }
    else{
        yDirection = 0 ;
    }

    if(previousTile.second > nextTile.second) {
        xDirection = -1 ;
    }
    else if(previousTile.second < nextTile.second) {
        xDirection = 1 ;
    }
    else{
        xDirection = 0 ;
    }

    return ;
}