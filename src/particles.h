#pragma once
#include "config.h"
#include "vec3.h"

class Particles
{
private:
    unsigned int m_numberOfParticles;
public:
    float x[MAXNUMPARTICLES];
    float y[MAXNUMPARTICLES];
    float vx[MAXNUMPARTICLES];
    float vy[MAXNUMPARTICLES];

    Particles();
    unsigned int numberOfParticles() const;
    void setNumberOfParticles(unsigned int numberOfParticles);
    void maxwellianVelocity(unsigned int particleIndex, float temperature, float mass);
    void findPosition(unsigned int particleIndex, vec3 systemSize);
};
