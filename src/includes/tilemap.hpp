#ifndef TILEMAP_H
#define TILEMAP_H

void initializeTilemap(int *tilemap, int rows, int cols) ;

void displayTilemap(int *tilemap, int rows, int cols) ;

void setTilemap(int *tilemap, int rows, int cols, const char *filename) ;

void getTilemap(int *tilemap, int rows, int cols, const char *filename) ;

bool isWall(int *tilemap, int rows, int cols, int xpos, int ypos) ;

#endif