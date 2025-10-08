#ifndef TILEMAP_H
#define TILEMAP_H

void initializeTilemap() ;

void displayTilemap() ;

void setTilemap(const char *filename) ;

void getTilemap(const char *filename) ;

bool isWall(int xpos, int ypos) ;

#endif