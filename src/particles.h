#pragma once
#include "config.h"
#include "vec2.h"
#include <functional>
using std::function;
class Random; class Grid;
class Particles
{
private:
    unsigned int m_numberOfParticles;
public:
    double x[MAXNUMPARTICLES];
    double y[MAXNUMPARTICLES];
    double vx[MAXNUMPARTICLES];
    double vy[MAXNUMPARTICLES];

    Particles();
    unsigned int numberOfParticles() const;
    void setNumberOfParticles(unsigned int numberOfParticles);
    void maxwellianVelocity(unsigned int particleIndex, float temperature, float mass, Random *random);
    void findPosition(unsigned int particleIndex, vec2 systemSize, Random *random, Grid *grid);
    void for_each(std::function<void(float x, float y, float vx, float vy)> action);
    void for_each(std::function<void(float x, float y)> action);
    double velocitySquared(unsigned int particleIndex) { return vx[particleIndex]*vx[particleIndex] + vy[particleIndex]*vy[particleIndex]; }
};
