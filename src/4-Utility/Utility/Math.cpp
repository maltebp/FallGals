#include "Math.hpp"

#include <cmath>

using namespace fg::utility;


float Math::wrapDegrees(float degrees) {
    if( degrees > 0 ) {
        return std::fmod(degrees, 360.0f);
    } 

    if( degrees < 0 ) {
        return 360.0f - std::fmod(degrees * -1.0f, 360.0f);
    }

    if( degrees == 0 ) {
        return 0.0f;
    }
}

float Math::randomFloat(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}