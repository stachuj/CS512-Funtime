#include "level_manager.hpp"
#include "collectible.hpp"
#include <iostream>
#include <fstream>

// 30 level slots, 12 rows, 16 columns
int levels[30][12][16] = {0};

// Index of currently loaded level (0-based)
static int currentLevel = 0;

// -----------------------------------------------------------------------------
// Sets tilemap in tilemap.cpp to the specified level
// -----------------------------------------------------------------------------
void LoadLevel(int index) {

    if (index < 0 || index > 29)
        return;

    // Track which level is currently loaded
    currentLevel = index;

    for (int row = 0; row < 12; row++) {
        for (int col = 0; col < 16; col++) {
            setTile(row, col, levels[index][row][col]);
        }
    }

    return;
}

// -----------------------------------------------------------------------------
// Saves the current tilemap back into levels[index]
// -----------------------------------------------------------------------------
void SaveLevel(int index) {

    for (int row = 0; row < 12; row++) {
        for (int col = 0; col < 16; col++) {
            levels[index][row][col] = getTile(row, col);
        }
    }

    return;
}

// -----------------------------------------------------------------------------
// Load all 30 levels from a text file (like testLevels.txt)
// -----------------------------------------------------------------------------
void LoadFromFile(std::string filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: failed to open level file for reading." << std::endl;
        return;
    }

    for (int index = 0; index < 30; index++) {
        for (int row = 0; row < 12; row++) {
            for (int col = 0; col < 16; col++) {

                if (!(file >> levels[index][row][col])) {
                    // If the file ends early, just pad remaining with 0
                    levels[index][row][col] = 0;
                }
            }
        }
    }

    file.close();

    return;
}

// -----------------------------------------------------------------------------
// Save all 30 levels back to a text file (like testLevels.txt)
// -----------------------------------------------------------------------------
void SaveToFile(std::string filename) {

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: failed to save levels to file." << std::endl;
        return;
    }

    for (int index = 0; index < 30; index++) {

        for (int row = 0; row < 12; row++) {

            for (int col = 0; col < 16; col++) {

                file << levels[index][row][col] << " ";

            }

            file << std::endl;

        }

        file << std::endl;
        file << std::endl;

    }

    file.close();

    std::cout << "File saved successfully." << std::endl;

    return;
}

// -----------------------------------------------------------------------------
// New helper: query which level is currently loaded (0-based index)
// -----------------------------------------------------------------------------
int GetCurrentLevel() {
    return currentLevel;
}
