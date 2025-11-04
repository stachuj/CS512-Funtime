#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "tilemap.hpp"
#include <string>

void LoadLevel(int index) ;

void SaveLevel(int index) ;

void LoadFromFile(std::string path) ;

void SaveToFile(std::string filename) ;

#endif