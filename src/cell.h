#pragma once
#include "config.h"
#include "particles.h"

class Random;
class Cell
{
private:
    Particles m_particles;
    unsigned long m_numberOfCollisionTrials;
    unsigned long m_numberOfCollisions;
    float m_collisionRest;
    float m_collisionCoefficient;
    float m_volume;
    float m_maxRelativeVelocitySquared;
    void collideParticles(unsigned int i, unsigned int j, const float relativeVelocityHalf, const float randomNumber);
public:
    Cell();
    void setVolume(float volume, unsigned int numberOfAtomsPerParticle, float atomDiameter);
    unsigned long collide(float dt, Random *random);
    unsigned int addParticle(float x, float y, float vx=0, float vy=0);
    void removeParticle(unsigned int particleIndex);
    unsigned int numberOfParticles() { return m_particles.numberOfParticles(); }
    void updateMaxRelativeVelocity();
    unsigned int numberOfCollisionTrials() { return m_numberOfCollisionTrials; }
    Particles *particles() { return &m_particles; }
};
