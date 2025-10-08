#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>


//Init tilemap
int tilemap[12][16] = {1};
int rows = 12;
int cols = 16;

/*
Basic function for initializing the tilemap.
For now, sets the perimeter of the tilemap to 1 (walls).
Can be updated later for a more complex tilemap. 
*/
void initializeTilemap() {

    for (int i = 0 ; i < rows ; i++) {

        for (int j = 0 ; j < cols ; j++) {

            if(i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) {

                tilemap[i][j] = 1 ;

            }

        }

    }

    tilemap[4][3] = 1;

    tilemap[7][6] = 1;

    tilemap[10][8] = 1;

    return ;

}

/*
Basic function to display the tilemap.
Makes walls red, and draws an outline around each tile. 
May change if statement to a switch statement later to 
make it easier to display other tile types. 
*/

void displayTilemap() {

    for (int i = 0 ; i < rows ; i++) {

        for (int j = 0 ; j < cols ; j++) {

            if(tilemap[i][j] == 1) {

                 DrawRectangle(j * 50, i * 50, 50, 50, BLACK) ;

            }

            DrawRectangleLines(j * 50, i * 50, 50, 50, ColorAlpha(BLUE, 0.1)) ;

        }

    }

    return ;

}

/*
Saves the tilemap as a 2d array of ints to a text file.
Currently saves to the debug folder, unsure if this will cause issues later.
*/

void setTilemap(const char *filename) {

    std::ofstream file(filename, std::ofstream::trunc) ;

    if(file.is_open()) {

        for (int i = 0 ; i < rows ; i++) {

            for (int j = 0 ; j < cols ; j++) {

                file << *tilemap[j + cols * i] << " " ;

            }

            file << std::endl ;

        }

        file.close() ;

        std::cout << "File saved successfully." ;

    }
    else {

        std::cerr << "ERROR: failed to save tilemap to file." ;

    }

    tilemap[0][0] = 0;

    return ;

}

/*
Gets tilemap from file and saves it to a 2d array.
*/

void getTilemap(const char *filename) { 

    std::ifstream file(filename) ;

    if (file.is_open()) {

        for (int i = 0 ; i < rows ; i++) {

            for (int j = 0 ; j < cols ; j++) {

                file >> *tilemap[j + cols * i] ;

            }

        }

        file.close() ;

        std::cout << "File read successfully." ;

    }
    else {

        std::cerr << "ERROR: failed to read tilemap from file." ;

    }

    return ;

}

/*
Checks area of tilemap the x and y coordinates are in.
Returns true if it's a wall, false if not. 
Could maybe update to be more general in the future (return what type of tile).
*/

bool isWall(int xpos, int ypos) {

    int colpos = xpos / 50 ;
    int rowpos = ypos / 50 ;

    if(tilemap[rowpos][colpos] == 1) {
        return true ;
    }
    return false ;
    
}