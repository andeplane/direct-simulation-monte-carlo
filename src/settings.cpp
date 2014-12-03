#include "settings.h"

Settings::Settings() :
    systemSize(vec3(1,1,0)),
    loadState(false),
    atomsPerParticle(10),
    particlesPerCell(10),
    density(1e6),
    mass(39.948),
    atomDiameter(0.000362),
    temperature(3.0)
{
}
