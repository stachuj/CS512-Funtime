#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>

/*
Basic function for initializing the tilemap.
For now, sets the perimeter of the tilemap to 1 (walls).
Can be updated later for a more complex tilemap. 
*/
void initializeTilemap(int *tilemap, int rows, int cols) {

    for (int i = 0 ; i < rows ; i++) {

        for (int j = 0 ; j < cols ; j++) {

            if(i == 0 || j == 0 || i == (rows - 1) || j == (cols - 1)) {

                tilemap[j + cols * i] = 1 ;

            }

        }

    }

    tilemap[4 + cols * 3] = 1 ;

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

            if(tilemap[j + cols * i] == 1) {

                 DrawRectangle(j * 50, i * 50, 50, 50, RED) ;

            }

            DrawRectangleLines(j * 50, i * 50, 50, 50, BLACK) ;

        }

    }

    return ;

}

/*
Saves the tilemap as a 2d array of ints to a text file.
Currently saves to the debug folder, unsure if this will cause issues later.
*/

void setTilemap(int *tilemap, int rows, int cols, const char *filename) {

    std::ofstream file(filename, std::ofstream::trunc) ;

    if(file.is_open()) {

        for (int i = 0 ; i < rows ; i++) {

            for (int j = 0 ; j < cols ; j++) {

                file << tilemap[j + cols * i] << " " ;

            }

            file << std::endl ;

        }

        file.close() ;

        std::cout << "File saved successfully." ;

    }
    else {

        std::cerr << "ERROR: failed to save tilemap to file." ;

    }

    return ;

}

/*
Gets tilemap from file and saves it to a 2d array.
*/

void getTilemap(int *tilemap, int rows, int cols, const char *filename) { 

    std::ifstream file(filename) ;

    if (file.is_open()) {

        for (int i = 0 ; i < rows ; i++) {

            for (int j = 0 ; j < cols ; j++) {

                file >> tilemap[j + cols * i] ;

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

bool isWall(int *tilemap, int rows, int cols, int xpos, int ypos) {

    int colpos = xpos / 50 ;
    int rowpos = ypos / 50 ;

    if(tilemap[colpos + cols * rowpos] == 1) {
        return true ;
    }
    return false ;
    
}