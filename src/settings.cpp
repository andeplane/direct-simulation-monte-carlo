#include "settings.h"

Settings::Settings() :
    systemSize(vec2(1,1)),
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
