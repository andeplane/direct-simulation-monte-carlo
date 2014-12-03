#pragma once
#include "vec3.h"

class Settings
{
public:
    vec3 systemSize;
    bool loadState;
    unsigned int atomsPerParticle;
    float density;
    float mass;
    float atomDiameter;
    float temperature;
    Settings();
};
