#pragma once
#include "config.h"

class Particles; class Random;
class Cell
{
private:
    unsigned int  m_particleIndices[MAXNUMPARTICLESPERCELL];
    unsigned int m_numberOfParticles;
    unsigned long m_numberOfCollisionTrials;
    unsigned long m_numberOfCollisions;
    double m_collisionRest;
    double m_collisionCoefficient;
    double m_volume;
    double m_maxRelativeVelocitySquared;
    void collideParticles(double &vxi, double &vyi, double &vxj, double &vyj, const double relativeVelocityHalf, const double randomNumber);
public:
    Cell();
    void setVolume(double volume, unsigned int numberOfAtomsPerParticle, double atomDiameter);
    unsigned long collide(double dt, Particles *particles, Random *random);
    void addParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    void removeParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    unsigned int numberOfParticles() { return m_numberOfParticles; }
    void updateMaxRelativeVelocity(Particles *particles);
    unsigned int numberOfCollisionTrials() { return m_numberOfCollisionTrials; }
};
