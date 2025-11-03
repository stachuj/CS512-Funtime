#include "tilemap.hpp"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <set>

int tilemap[12][16] = {1};
int tilemapRows = 12;
int tilemapCols = 16;

//Init tilemap

/*
Basic function for initializing the tilemap.
For now, sets the perimeter of the tilemap to 1 (walls).
Can be updated later for a more complex tilemap. 
*/
void initializeTilemap() {

    for (int i = 0 ; i < tilemapRows ; i++) {

        for (int j = 0 ; j < tilemapCols ; j++) {

            if(i == 0 || j == 0 || i == (tilemapRows - 1) || j == (tilemapCols - 1)) {

                tilemap[i][j] = 1 ;

            }

        }

    }

    tilemap[4][3] = 1;

    tilemap[7][6] = 1;

    tilemap[10][8] = 1;

    return ;

}

void initializeAStarTestTilemap() {

    for (int i = 0 ; i < tilemapRows ; i++) {

        for (int j = 0 ; j < tilemapCols ; j++) {

            if(i == 0 || j == 0 || i == (tilemapRows - 1) || j == (tilemapCols - 1)) {

                tilemap[i][j] = 1 ;

            }

            if(j == 4 && i < 10) {
                tilemap[i][j] = 1 ;
            }

            if(j == 8 && i > 2) {
                tilemap[i][j] = 1 ;
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

void displayTilemap() {

    for (int i = 0 ; i < tilemapRows ; i++) {

        for (int j = 0 ; j < tilemapCols ; j++) {

            if(tilemap[i][j] == 1) {

                DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK) ;

            }

            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(BLUE, 0.1)) ;

        }

    }

    return ;

}

/*
Saves the tilemap as a 2d array of ints to a text file.
Currently saves to the debug folder, unsure if this will cause issues later.
*/

void LoadTilemap(const char *filename) {

    std::ofstream file(filename, std::ofstream::trunc) ;

    if(file.is_open()) {

        for (int i = 0 ; i < tilemapRows ; i++) {

            for (int j = 0 ; j < tilemapCols ; j++) {

                file << *tilemap[j + tilemapCols * i] << " " ;

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

void SaveTilemap(const char *filename) { 

    std::ifstream file(filename) ;

    if (file.is_open()) {

        for (int i = 0 ; i < tilemapRows ; i++) {

            for (int j = 0 ; j < tilemapCols ; j++) {

                file >> *tilemap[j + tilemapCols * i] ;

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

// Converts position on the window to the tile in the tilemap. 

int getTilePos(float pos) {
    int tilePos = pos / TILE_SIZE ;

    return tilePos ;
}

/*
Checks area of tilemap the x and y coordinates are in.
Returns true if it's a wall, false if not. 
Could maybe update to be more general in the future (return what type of tile).
*/

bool isWall(float xpos, float ypos) {

    int colpos = getTilePos(xpos) ;
    int rowpos = getTilePos(ypos) ;

    if(tilemap[rowpos][colpos] == 1) {
        return true ;
    }
    return false ;
    
}

int getRows() {
    return tilemapRows ;
}

int getCols() {
    return tilemapCols ;
}

int getTile(int row, int col) {
    return tilemap[row][col] ;
} 

bool isValid(int row, int col) {
    if (row < 0 || row >= getRows() || col < 0 || col >= getCols()) {
        return false ;
    }
    return true ;
}

double calculateHValue(int xCurrent, int yCurrent, int xDestination, int yDestination) {

    double h = abs(xCurrent - xDestination) + abs(yCurrent - yDestination) ;

    return h ;

}

std::stack<Pair> AStarSearch(int rowStart, int colStart, int colDestination, int rowDestination) {

    std::stack<Pair> Path ;

    if(!isValid(rowStart, colStart) || !isValid(rowDestination, colDestination)) {
        return Path ;
    }

    if(getTile(rowStart, colStart) == 1 || getTile(rowDestination, colDestination) == 1) {
        return Path ;
    }

    if(rowStart == rowDestination && colStart == colDestination) {
        return Path ;
    }

    bool closedList[12][16] = {false} ;

    cell cellDetails[12][16] ;

    int i, j ;

    for (i = 0; i < tilemapRows ; i++) {
        for (j = 0; j < tilemapCols ; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = rowStart, j = colStart;
    cellDetails[i][j].f = 0.0 ;
    cellDetails[i][j].g = 0.0 ;
    cellDetails[i][j].h = 0.0 ;
    cellDetails[i][j].parent_i = i ;
    cellDetails[i][j].parent_j = j ;

    std::set<AStarPair> openList ;
    openList.insert(std::make_pair(0.0, std::make_pair(i, j))) ;

    bool foundDest = false;

    while (!openList.empty()) {

        AStarPair p = *openList.begin();
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        //printf("A Star on tile (%d, %d)\n", i, j) ;

        double gNew, hNew, fNew;

        // Check if North cell is valid
        if (isValid(i - 1, j) == true) {
            // If we're at our destination, make our path and return it
            if ((i - 1) == rowDestination && j == colDestination) {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                Path = tracePath(cellDetails, rowDestination, colDestination);
                foundDest = true;
                return Path ;
            }
            // Else we calculate the f, g, and h values and add it to the list
            // if it isn't on there or the f value is better. 
            else if (closedList[i - 1][j] == false && tilemap[i - 1][j] != 1) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, rowDestination, colDestination);
                fNew = gNew + hNew;
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));
                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        // Check if South cell is valid
        if (isValid(i + 1, j) == true) {
            // If we're at our destination, make our path and return it
            if ((i + 1) == rowDestination && j == colDestination) {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                Path = tracePath(cellDetails, rowDestination, colDestination);
                foundDest = true;
                return Path ;
            }
            // Else we calculate the f, g, and h values and add it to the list
            // if it isn't on there or the f value is better.
            else if (closedList[i + 1][j] == false && tilemap[i + 1][j] != 1) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, rowDestination, colDestination);
                fNew = gNew + hNew;
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        // Check if East cell is valid
        if (isValid(i, j + 1) == true) {
            // If we're at our destination, make our path and return it
            if (i == rowDestination && (j + 1) == colDestination) {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                Path = tracePath(cellDetails, rowDestination, colDestination);
                foundDest = true;
                return Path ;
            }
            // Else we calculate the f, g, and h values and add it to the list
            // if it isn't on there or the f value is better.
            else if (closedList[i][j + 1] == false && tilemap[i][j + 1] != 1) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, rowDestination, colDestination);
                fNew = gNew + hNew;
                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j + 1)));
                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        // Check if West cell is valid
        if (isValid(i, j - 1) == true) {
            // If we're at our destination, make our path and return it
            if (i == rowDestination && (j - 1) == colDestination) {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                Path = tracePath(cellDetails, rowDestination, colDestination);
                foundDest = true;
                return Path ;
            }
            // Else we calculate the f, g, and h values and add it to the list
            // if it isn't on there or the f value is better.
            else if (closedList[i][j - 1] == false && tilemap[i][j - 1] != 1) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, rowDestination, colDestination);
                fNew = gNew + hNew;
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(std::make_pair(fNew, std::make_pair(i, j - 1)));
                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //printf("End of A Star on tile (%d, %d)\n", i, j) ;
    }

    return Path ;
}

std::stack<Pair> tracePath(cell cellDetails[12][16], int rowDestination, int colDestination) {

    std::stack<Pair> Path ;

    int row = rowDestination ;
    int col = colDestination ;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        printf("Adding (%d,%d) \n", row, col) ;
        Path.push(std::make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i ;
        int temp_col = cellDetails[row][col].parent_j ;
        row = temp_row ;
        col = temp_col ;
    }

    return Path ;

}

void displayPath(std::stack<Pair> Path) {

    while(!Path.empty()) {
        Pair p = Path.top() ;
		Path.pop() ;
        DrawRectangle(p.second * TILE_SIZE, p.first * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(GREEN, 0.5)) ;
    }

    return ;
}