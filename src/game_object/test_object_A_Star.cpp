#include "test_object_A_Star.hpp"

void TestObjectAStar::Update(float deltaTime) {
    //|| previousTile.first == -1
    if(checkOnTile()){
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

    setDirection() ;
    
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
    /*
    else {
        nextTile = previousTile ;
    }
        */
    setDirection() ;
    return ;
}

bool TestObjectAStar::checkOnTile() {
    /*
    int x = position.x - 16 ;
    int y = position.y - 16 ;
    int widthHeight = 32 ;
    return (getTilePos(x) == nextTile.second &&
            getTilePos(x + widthHeight) == nextTile.second && 
            getTilePos(y) == nextTile.first && 
            getTilePos(y + widthHeight) == nextTile.first) ;
    */

    double distanceFromEdge = 20.0 ;
    double xPos = nextTile.second * 50.0 ;
    double yPos = nextTile.first * 50.0 ;

    return((xPos + distanceFromEdge <= position.x) &&
            (position.x <= xPos + 50 - distanceFromEdge) &&
            (yPos + distanceFromEdge <= position.y) &&
            (position.y <= yPos + 50 - distanceFromEdge)) ;
}

void TestObjectAStar::setDirection() {
    /*
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
    */

    double distanceFromEdge = 20.0 ;
    double xPos = nextTile.second * 50.0 ;
    double yPos = nextTile.first * 50.0 ;

    if(position.x < xPos + distanceFromEdge){
        xDirection = 1 ;
    }
    else if(position.x > xPos + 50.0 - distanceFromEdge){
        xDirection = -1 ;
    }
    else{
        xDirection = 0 ;
    }

    if(position.y < yPos + distanceFromEdge){
        yDirection = 1 ;
    }
    else if(position.y > yPos + 50.0 - distanceFromEdge){
        yDirection = -1 ;
    }
    else{
        yDirection = 0 ;
    }

    return ;
}