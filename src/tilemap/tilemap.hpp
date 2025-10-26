#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <stack>

struct cell {
            int parent_i, parent_j ;
            double f, g, h ;
        } ;
typedef std::pair<int, int> Pair ;
typedef std::pair<double, std::pair<int, int>> AStarPair ;

void initializeTilemap() ;

void initializeAStarTestTilemap() ;

void displayTilemap() ;

void setTilemap(const char *filename) ;

void getTilemap(const char *filename) ;

int getTilePos(float pos) ;

bool isWall(float xpos, float ypos) ;

int getRows() ;

int getCols() ;

int getTile(int row, int col) ;

std::stack<Pair> AStarSearch(int rowStart, int colStart, int rowDestination, int colDestination) ;

std::stack<Pair> tracePath(cell cellDetails[12][16], int rowDestination, int colDestination) ;

void displayPath(std::stack<Pair> Path) ;

#endif