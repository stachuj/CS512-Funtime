#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include "test_object_A_Star.hpp"

#include <vector>
#include <raylib.h>

#include "game_state.hpp"     // ADDED (score/timer state)
#include "hud.hpp"           // ADDED (draw score/timer)
#include "collectible.hpp"   // ADDED (collectibles)
#include <string>

using namespace std;

Chat chat;

int main() {

    // list of all objects that will be updated and drawn
    // they are pointers so we can have subclasses in the vector
    std::vector<GameObject *> objects;

    initializeTilemap();
    //setTilemap("testfile.txt");
    //getTilemap("testfile.txt");

    InitAudioDevice();
	// initializeTilemap() ;
	initializeAStarTestTilemap() ;
	//setTilemap("testfile.txt") ;
	//getTilemap("testfile.txt") ;

	// Testing A Star to make sure it works
	// std::stack<Pair> Path = AStarSearch(1, 1, 9, 14) ;
	// std::stack<Pair> PathCopy = Path ;
    /*
	string PathString = "The path is: " ;
	int i = 0 ;
	while(!Path.empty()) {
		Pair p = Path.top() ;
		Path.pop() ;
		printf("-> (%d,%d) ", p.first, p.second) ;
		PathString += "(" + to_string(p.first) + ", " + to_string(p.second) + "), " ;
		i += 1 ;
		if(i == 9){
			PathString += "\n" ;
			i = 0 ;
		}
	}
    */

    // Variables for updating AStar
    std::stack<Pair> Path ;
    float testEnemyUpdate = 3.0 ;
    int mouseRowTile = 0 ;
    int mouseColTile = 0 ;

    Sound scream = LoadSound("assets/scream.wav");
    Sound pew    = LoadSound("assets/pew.wav");
    Sound mew    = LoadSound("assets/mew.wav");

    // ADDED: pickup SFX for collectibles (reuse existing)
    Sound pickupSfx = pew;

    // Window size
    InitWindow(800, 600, "CS512 Funtime");
    SetTargetFPS(60);

    // Adding a new object to the world
    // Will have to come up with a nicer way to create and remove objects from the world...
    Character* player = new Character({600, 300}, std::string("../../assets"));
    objects.push_back(player);
    objects.push_back(new TestObject({100.0, 400.0}));
    TestObjectAStar* testEnemy = new TestObjectAStar({80.0, 80.0}) ;
    objects.push_back(testEnemy) ;

    // ADDED: game state + collectibles
    GameState gs;                               // holds score & timer
    std::vector<Collectible> collectibles;      // pickups to score
    Collectibles::SpawnRandom(collectibles, 10, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});

    // ADDED: chat toggle (hidden by default)
    bool chatVisible = false;

    PlaySound(scream);
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP))   PlaySound(scream);
        if (IsKeyPressed(KEY_DOWN)) PlaySound(pew);
        if (IsKeyPressed(KEY_LEFT)) PlaySound(mew);

        // Toggle chat overlay with C
        if (IsKeyPressed(KEY_C)) chatVisible = !chatVisible;

        // This is the real time in seconds since the last update
        float deltaTime = GetFrameTime();

        // ADDED: countdown timer
        if (!gs.timeUp) {
            gs.timeRemaining -= deltaTime;
            if (gs.timeRemaining <= 0.0f) {
                gs.timeRemaining = 0.0f;
                gs.timeUp = true;
            }
        }

        // Update AStar for testEnemy every 3 seconds
        testEnemyUpdate -= deltaTime ;
        Vector2 mouse = GetMousePosition() ;
        if(!isWall(mouse.x, mouse.y)){
            mouseColTile = getTilePos(mouse.x) ;
            mouseRowTile = getTilePos(mouse.y) ;
        }
        if(testEnemyUpdate <= 0.0) {
            testEnemyUpdate = 3.0 ;
            int testEnemyColTile = getTilePos(testEnemy->position.x) ;
            int testEnemyRowTile = getTilePos(testEnemy->position.y) ;
            Path = AStarSearch(testEnemyRowTile, testEnemyColTile, mouseRowTile, mouseColTile) ;
            testEnemy->setPath(Path) ;
        }

        // Update game objects
        for (auto object: objects)
            object->Update(deltaTime);

        // ADDED: player bounds for pickup collision
        Rectangle playerBounds;
        Vector2 p  = player->GetPosition();
        Vector2 sz = player->GetSize();
        playerBounds = { p.x - sz.x * 0.5f, p.y - sz.y * 0.5f, sz.x, sz.y };

        if (!gs.timeUp) {
            int newlyPicked = Collectibles::Update(collectibles, &playerBounds, pickupSfx);
            gs.score += newlyPicked * gs.pointsPerCollectible;
        }

        // Optional restart after time up
        if (gs.timeUp && IsKeyPressed(KEY_R)) {
            gs.score = 0;
            gs.timeRemaining = gs.timeLimit;
            gs.timeUp = false;
            for (auto& c : collectibles) c.active = true; // reactivate all
        }
        
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // ---- WORLD (no camera mode here) ----
            displayPath(Path);
            displayTilemap();
            displayPath(PathCopy) ;

            for (auto object: objects)
                object->Draw();

            // ---- UI (screen space) ----
            // Draw collectibles ABOVE world (so always visible), then HUD, then optional chat on top
            Collectibles::Draw(collectibles);

            // Draw HUD before chat so chat panel never covers score/time
            DrawHUD(gs);


        }
        EndDrawing();
    }

    // Cleanup
    UnloadSound(scream);
    UnloadSound(pew);
    UnloadSound(mew);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
