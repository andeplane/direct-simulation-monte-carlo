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
    float m_collisionRest;
    float m_collisionCoefficient;
    float m_volume;
    float m_maxRelativeVelocitySquared;
    void collideParticles(float &vxi, float &vyi, float &vxj, float &vyj, const float relativeVelocityHalf, const float randomNumber);
public:
    Cell();
    void setVolume(float volume, unsigned int numberOfAtomsPerParticle, float atomDiameter);
    unsigned long collide(float dt, Particles *particles, Random *random);
    void addParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    void removeParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    unsigned int numberOfParticles() { return m_numberOfParticles; }
    void updateMaxRelativeVelocity(Particles *particles);
    unsigned int numberOfCollisionTrials() { return m_numberOfCollisionTrials; }
};
