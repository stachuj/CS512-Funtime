#ifndef LEVEL_SELECT_HPP
#define LEVEL_SELECT_HPP
#include "raylib.h"

class LevelSelect {
    private:
        int level = 0 ;
        int selected = 0 ;
    public: 
        LevelSelect() {} ;
        int Update() ;
        void Draw() ;
        int GetLevel() ;
};

#endif