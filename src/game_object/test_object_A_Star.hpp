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
        float moveSpeed = 200.0f; // pixels per second

        float collisionWidth = 32.0;
        float collisionHeight = 32.0;

        bool awake = false;

        // --- sprite / animation ---
        Texture2D spriteSheet = {0};     // sprite sheet texture
        Vector2 frameSize = {0, 0};      // width/height of single frame
        int framesPerRow = 3;
        int rows = 4;                    // up, right, down, left
        int currentFrame = 0;            // 0..framesPerRow-1
        int currentRow = 2;              // default to down row (2)
        float animTimer = 0.0f;
        float frameTime = 0.12f;         // seconds per frame
        bool ownsTexture = false;  

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

        ~TestObjectAStar() {
            // if this object owns the texture, free it
            if (ownsTexture && spriteSheet.id != 0) {
                UnloadTexture(spriteSheet);
            }
        }

        // set sprite sheet and frame sizing. If ownTex is true then destructor will unload texture.
        void setSpriteSheet(const Texture2D &tex, bool ownTex = false) {
            spriteSheet = tex;
            ownsTexture = ownTex;
            if (tex.id != 0) {
                frameSize.x = (float)tex.width / framesPerRow;
                frameSize.y = (float)tex.height / rows;
            } else {
                frameSize = {0,0};
            }
        }

        void Update(float deltaTime) override ;
        void Draw() override;
        void setPath(std::stack<Pair> newPath) ;
        void setNextTile() ;
        bool checkOnTile() ;
        float spriteScale = 2.0f; 

        private:
        // helper to update animation row based on velocity
        void updateAnimationDirection();
} ;

#endif