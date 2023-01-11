#pragma once

#include <fstream>

#define CBE_DEBUG

namespace CBE {
    bool attackerLoaded = false;
    bool defenderLoaded = false;
    float baseAccuracy = 50.1;
    extern std::fstream debugFile;

    const int AMMO_EMPTY = 0;
    const int AMMO_INFINITE = -1;
}