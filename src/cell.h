#pragma once
#include "config.h"

class Particles;
class Cell
{
private:
    unsigned int  m_particleIndices[MAXNUMPARTICLESPERCELL];
    unsigned int m_numberOfParticles;
    unsigned long m_numberOfCollisions;
    float m_collisionRest;
    float m_collisionCoefficient;
    float m_volume;
    float m_maxRelativeVelocity;
    void collideParticles(float &vxi, float &vyi, float &vxj, float &vyj, const float relativeVelocity);
public:
    Cell();
    void setVolume(float volume, unsigned int numberOfAtomsPerParticle, float atomDiameter);
    unsigned long collide(float dt, Particles *particles);
    void addParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    void removeParticle(unsigned int particleIndex, unsigned int *particleIndexMap);
    unsigned int numberOfParticles() { return m_numberOfParticles; }
    void updateMaxRelativeVelocity(Particles *particles);
};
