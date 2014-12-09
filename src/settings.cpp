#include "settings.h"
#include <cmath>

Settings::Settings() :
    systemSize(vec2(0.25,0.25)),
    loadState(false),
    recomputeMaxRelativeVelocityEvery(100),
    atomsPerParticle(10),
    particlesPerCell(10),
    density(1e6),
    mass(39.948),
    atomDiameter(0.000362),
    temperature(3.0)
{
}

double Settings::meanFreePath()
{
    return 1.0/(sqrt(2)*2.0*atomDiameter*density*density);
}
