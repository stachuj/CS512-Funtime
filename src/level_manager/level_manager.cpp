#include "level_manager.hpp"
#include "collectible.hpp"
#include <iostream>
#include <fstream>

// 30 level slots, 12 rows, 16 columns
int levels[30][12][16] = {0};

// Sets tilemap in tilemap.cpp to the specified level
void LoadLevel(int index) {

    for(int row = 0 ; row < 12 ; row++) {
        for(int col = 0 ; col < 16 ; col++) {
            setTile(row, col, levels[index][row][col]) ;
        }
    }

    return ;
}

// Sets specified level to be the same as the tilemap in tilemap.cpp
void SaveLevel(int index) {

    for(int row = 0 ; row < 12 ; row++) {
        for(int col = 0 ; col < 16 ; col++) {
            levels[index][row][col] = getTile(row, col) ;
        }
    }

    return ;
}

// Load set of levels from filename
void LoadFromFile(std::string filename) {

    // first test wit just filename, then try with longer path to levels folder
    //std::string path = "../../levels/" + filename + ".txt" ;
    //std::ifstream file(path.c_str()) ;

    std::ifstream file(filename.c_str()) ;

    if(file.is_open()) {
        
        for(int index = 0 ; index < 30 ; index++) {

            for(int row = 0 ; row < 12 ; row++) {

                for(int col = 0 ; col < 16 ; col++) {

                    file >> levels[index][row][col] ;

                }

            }

        }

        file.close() ;

        std::cout << "File read successfully." ;
    }
    else {
        std::cerr << "ERROR: failed to read levels from file." ;
    }

    return ;
}

// Save set of levels to filename
void SaveToFile(std::string filename) {
    
    std::ofstream file(filename.c_str(), std::ofstream::trunc) ;

    if(file.is_open()) {

        for(int index = 0 ; index < 30 ; index++) {

            for(int row = 0 ; row < 12 ; row++) {

                for(int col = 0 ; col < 16 ; col++) {

                    file << levels[index][row][col] << " " ;

                }

                file << std::endl ;

            }

            file << std::endl ;
            file << std::endl ;

        }

        file.close() ;

        std::cout << "File saved successfully." ;

    }
    else {
        std::cerr << "ERROR: failed to save levels to file." ;
    }

    return ;
}