#pragma once
#include "vec3.h"

class Settings
{
public:
    vec3 systemSize;
    bool loadState;
    unsigned int atomsPerParticle;
    unsigned int particlesPerCell;
    float density;
    float mass;
    float atomDiameter;
    float temperature;
    Settings();

    float volume() { return systemSize[0]*systemSize[1]; }
    unsigned long numberOfAtoms() { return density * volume(); }
    unsigned int numberOfParticles() { return numberOfAtoms() / atomsPerParticle; }
    unsigned int numberOfCells() { return numberOfParticles() / particlesPerCell; }
};
