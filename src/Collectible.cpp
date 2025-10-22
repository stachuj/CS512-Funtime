#include "Collectible.h"
#include <algorithm>
#include <cmath>

static bool CircleRectOverlap(Vector2 c, float r, Rectangle rc) {
    float nx = fmaxf(rc.x, fminf(c.x, rc.x + rc.width));
    float ny = fmaxf(rc.y, fminf(c.y, rc.y + rc.height));
    float dx = c.x - nx, dy = c.y - ny;
    return (dx*dx + dy*dy) <= (r*r);
}

void Collectibles::SpawnRandom(std::vector<Collectible>& out, int count, Rectangle bounds) {
    for (int i = 0; i < count; ++i) {
        Collectible c;
        c.radius = 12.0f;
        c.active = true;
        c.pos.x = bounds.x + c.radius + GetRandomValue(0, (int)(bounds.width  - 2*c.radius));
        c.pos.y = bounds.y + c.radius + GetRandomValue(0, (int)(bounds.height - 2*c.radius));
        out.push_back(c);
    }
}

void Collectibles::Draw(const std::vector<Collectible>& items) {
    for (auto const& c : items) {
        if (!c.active) continue;
        DrawCircleV(c.pos, c.radius, GOLD);
        DrawCircleLines((int)c.pos.x, (int)c.pos.y, c.radius, BROWN);
    }
}

int Collectibles::Update(std::vector<Collectible>& items, const Rectangle* playerBoundsOpt, Sound pickupSfx) {
    int picked = 0;
    if (playerBoundsOpt) {
        Rectangle pb = *playerBoundsOpt;
        for (auto& c : items) {
            if (!c.active) continue;
            if (CircleRectOverlap(c.pos, c.radius, pb)) {
                c.active = false; picked++;
            }
        }
    } else {
        Vector2 m = GetMousePosition();
        for (auto& c : items) {
            if (!c.active) continue;
            float dx = c.pos.x - m.x, dy = c.pos.y - m.y;
            if ((dx*dx + dy*dy) <= (c.radius*c.radius)) {
                c.active = false; picked++;
            }
        }
    }
    if (picked > 0 && IsAudioDeviceReady()) PlaySound(pickupSfx);
    return picked;
}
