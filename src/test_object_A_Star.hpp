#ifndef TEST_OBJECT_A_STAR_HPP
#define TEST_OBJECT_A_STAR_HPP

#include "test_object.hpp"
#include "tilemap.hpp"
#include <cstdio>

class TestObjectAStar : public virtual TestObject {

    public:
        std::stack<Pair> path ;
        Pair nextTile  ;
        //Pair previousTile = std::make_pair(-1, -1) ;
        float xDirection, yDirection = 0 ;

        TestObjectAStar() {} ;

        TestObjectAStar(Vector2 pos) {
            this->position = pos ;
            this-> nextTile = std::make_pair(getTilePos(position.y), getTilePos(position.x)) ;
            printf("Starting tile: (%d, %d)",getTilePos(position.y), getTilePos(position.x)) ;
        } ;

        TestObjectAStar(Vector2 pos, std::stack<Pair> pa) {
            this->position = pos ;
            this-> path = pa ;
            this-> nextTile = std::make_pair(getTilePos(position.y), getTilePos(position.x)) ;
            printf("Starting tile: (%d, %d)",getTilePos(position.y), getTilePos(position.x)) ;
        } ;

        void Update(float deltaTime) override ;
        void setPath(std::stack<Pair> newPath) ;
        void setNextTile() ;
        bool checkOnTile() ;
        void setDirection() ;
} ;

#endif