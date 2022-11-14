#pragma once

#include <fstream>

namespace CBE {
    bool attackerLoaded = false;
    bool defenderLoaded = false;
    float baseAccuracy = 50.1;
    extern std::ofstream debugFile;
}