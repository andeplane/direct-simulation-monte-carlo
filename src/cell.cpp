#include "cell.h"
#include "particles.h"
#include "random.h"
#include <cmath>
#include <algorithm>
#include <cassert>

Cell::Cell() :
    m_numberOfParticles(0),
    m_numberOfCollisionTrials(0),
    m_numberOfCollisions(0),
    m_collisionRest(0),
    m_collisionCoefficient(0),
    m_volume(0),
    m_maxRelativeVelocitySquared(0)
{
}

void Cell::setVolume(double volume, unsigned int numberOfAtomsPerParticle, double atomDiameter)
{
    m_volume = volume;
    m_collisionCoefficient = numberOfAtomsPerParticle*atomDiameter / volume;
}

void Cell::collideParticles(double &vxi, double &vyi, double &vxj, double &vyj, const double relativeVelocityHalf, const double randomNumber) {
    const double vcmx = 0.5f*(vxi + vxj); // Center of mass velocity
    const double vcmy = 0.5f*(vyi + vyj);

    const double cosTheta = 1.0f - 2.0f*randomNumber;
    const double sinTheta = sqrt(1.0f - cosTheta*cosTheta);

    const double vrelxHalf = relativeVelocityHalf*sinTheta;
    const double vrelyHalf = relativeVelocityHalf*cosTheta;

    vxi = vcmx + vrelxHalf;
    vyi = vcmy + vrelyHalf;
    vxj = vcmx - vrelxHalf;
    vyj = vcmy - vrelyHalf;
}

unsigned long Cell::collide(double dt, Particles *particles, Random *random) {
    if(m_numberOfParticles < 2) return 0;
    // Compute how many collision candidates to perform
    double numberOfCollisionTrials = m_collisionCoefficient*m_numberOfParticles*(m_numberOfParticles-1)*sqrt(m_maxRelativeVelocitySquared)*dt + m_collisionRest;
    unsigned int numberOfCollisionTrialsRounded = round(numberOfCollisionTrials);
    m_collisionRest = numberOfCollisionTrials - numberOfCollisionTrialsRounded;
    double *vx = &particles->vx[0];
    double *vy = &particles->vy[0];
    m_numberOfCollisionTrials += numberOfCollisionTrialsRounded;

    for(unsigned int collision=0; collision<numberOfCollisionTrialsRounded; collision++) {
        // Use the generateSSE4 function to generate the 4 random numbers we need in this collision. They are all unsigned ints, so we need to normalize with random->normalizationFactor.
        unsigned int rands[4];
        random->generateSSE4(rands);
        const unsigned int localParticleIndex1 = rands[0]*random->normalizationFactor*m_numberOfParticles;
        const unsigned int localParticleIndex2 = (localParticleIndex1 + ( (unsigned int)(rands[1]*random->normalizationFactor* (m_numberOfParticles - 1) ))) % m_numberOfParticles;

        const unsigned int i = m_particleIndices[localParticleIndex1];
        const unsigned int j = m_particleIndices[localParticleIndex2];
        const double dvx = vx[i] - vx[j];
        const double dvy = vy[i] - vy[j];
        const double relativeVelocitySquared = dvx*dvx + dvy*dvy;

        if(relativeVelocitySquared > m_maxRelativeVelocitySquared) {
            m_maxRelativeVelocitySquared = relativeVelocitySquared;
        }

        float randomNumber = rands[2]*random->normalizationFactor;

        if(relativeVelocitySquared > randomNumber*randomNumber*m_maxRelativeVelocitySquared) {
            const double relativeVelocityHalf = 0.5*sqrt(relativeVelocitySquared);
            m_numberOfCollisions++;
            collideParticles(vx[i], vy[i], vx[j], vy[j], relativeVelocityHalf, rands[3]*random->normalizationFactor);
        }
    }

    return m_numberOfCollisions;
}

void Cell::addParticle(unsigned int particleIndex, unsigned int *particleIndexMap)
{
    m_particleIndices[m_numberOfParticles] = particleIndex;
    particleIndexMap[particleIndex] = m_numberOfParticles;
    m_numberOfParticles++;
#ifdef DSMC_DEBUG
    assert(m_numberOfParticles <= MAXNUMPARTICLESPERCELL && "Too many particles in a cell.");
#endif
}

void Cell::removeParticle(unsigned int particleIndex, unsigned int *particleIndexMap)
{
    unsigned int lastParticleIndex = m_particleIndices[m_numberOfParticles]; // We will swap this particle with the one to remove, so we need to update the index map
    unsigned int localParticleIndex = particleIndexMap[particleIndex]; // What is the local index of the particle to be removed?
    std::swap(m_particleIndices[localParticleIndex], m_particleIndices[m_numberOfParticles]); // Swap those two internally
    particleIndexMap[lastParticleIndex] = localParticleIndex; // Update the index map since we swapped the particles
    m_numberOfParticles--; // By reducing the number of particles, the particle is safely removed
}

void Cell::updateMaxRelativeVelocity(Particles *particles) {
    m_maxRelativeVelocitySquared = 0;
    for(unsigned int i=0; i<m_numberOfParticles; i++) {
        for(unsigned int j=i+1; j<m_numberOfParticles; j++) {
            double dvx = particles->vx[i] - particles->vx[j];
            double dvy = particles->vy[i] - particles->vy[j];
            double dv = dvx*dvx + dvy*dvy;
            m_maxRelativeVelocitySquared = std::max(m_maxRelativeVelocitySquared, dv);
        }
    }
}
