#pragma once
#include "vec2.h"

class Settings
{
public:
    vec2 systemSize;
    bool loadState;
    unsigned int recomputeMaxRelativeVelocityEvery;
    unsigned int atomsPerParticle;
    unsigned int particlesPerCell;
    double density;
    double mass;
    double atomDiameter;
    double temperature;
    Settings();

    double volume() { return systemSize[0]*systemSize[1]; }
    unsigned long numberOfAtoms() { return density * volume(); }
    unsigned int numberOfParticles() { return numberOfAtoms() / atomsPerParticle; }
    unsigned int numberOfCells() { return numberOfParticles() / particlesPerCell; }
    double massPerParticle() { return mass*atomsPerParticle; }
    double meanFreePath();
};
