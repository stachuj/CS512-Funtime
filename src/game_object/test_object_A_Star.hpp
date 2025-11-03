#ifndef TEST_OBJECT_A_STAR_HPP
#define TEST_OBJECT_A_STAR_HPP

#include "test_object.hpp"
#include "tilemap.hpp"
#include "character.hpp"

class TestObjectAStar : public virtual TestObject {

    public:
        std::stack<Pair> path;
        Pair nextTile;
        float pathingTimer = 0.0f;

        Vector2 targetPosition = {0.0, 0.0};
        Vector2 velocity = {0.0, 0.0};
        float moveSpeed = 160.0f; // pixels per second

        float collisionWidth = 32.0;
        float collisionHeight = 32.0;

        bool awake = false;

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
        void Draw() override;
        void setPath(std::stack<Pair> newPath) ;
        void setNextTile() ;
        bool checkOnTile() ;
} ;

#endif