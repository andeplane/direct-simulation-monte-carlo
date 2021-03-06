#pragma once
#include "config.h"
#include "vec2.h"
#include <functional>
using std::function;
class Random;
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
    void maxwellianVelocity(unsigned int particleIndex, float temperature, float mass, Random *random);
    void findPosition(unsigned int particleIndex, vec2 systemSize, Random *random);
    void for_each(std::function<void(float x, float y, float vx, float vy)> action);
    void for_each(std::function<void(float x, float y)> action);
    float velocitySquared(unsigned int particleIndex) { return vx[particleIndex]*vx[particleIndex] + vy[particleIndex]*vy[particleIndex]; }
};
