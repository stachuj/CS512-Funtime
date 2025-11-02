#pragma once
#include <vector>
#include "raylib.h"

struct Collectible {
    Vector2 pos{0,0};
    float   radius = 12.0f;
    bool    active = true;
};

namespace Collectibles {
    void SpawnRandom(std::vector<Collectible>& out, int count, Rectangle bounds);
    void Draw(const std::vector<Collectible>& items);
    int  Update(std::vector<Collectible>& items, const Rectangle* playerBoundsOpt, Sound pickupSfx);
}
