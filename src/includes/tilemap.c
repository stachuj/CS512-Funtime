#include "raylib.h"

/*
Basic function for initializing the tilemap.
For now, sets the perimeter of the tilemap to 1 (walls).
Can be updated later for a more complex tilemap. 
*/
void initializeTilemap(int *tilemap, int rows, int cols) {

    for (int i = 0 ; i < rows ; i++) {

        for (int j = 0 ; j < cols ; j++) {

            if(i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) {

                tilemap[i + rows * j] = 1 ;

            }

        }

    }

    return ;

}

/*
Basic function to display the tilemap.
Makes walls red, and draws an outline around each tile. 
May change if statement to a switch statement later to 
make it easier to display other tile types. 
*/

void displayTilemap(int *tilemap, int rows, int cols) {

    for (int i = 0 ; i < rows ; i++) {

        for (int j = 0 ; j < cols ; j++) {

            if(tilemap[i + rows * j] == 1) {

                 DrawRectangle(i * 50, j * 50, 50, 50, RED) ;

            }

            DrawRectangleLines(i * 50, j * 50, 50, 50, BLACK) ;

        }

    }

    return ;

}