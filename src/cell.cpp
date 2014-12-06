#include "cell.h"
#include "particles.h"
#include "random.h"
#include <cmath>
#include <algorithm>

Cell::Cell() :
    m_numberOfParticles(0),
    m_numberOfCollisions(0),
    m_collisionRest(0),
    m_collisionCoefficient(0),
    m_volume(0),
    m_maxRelativeVelocity(0)
{
}

void Cell::setVolume(float volume, unsigned int numberOfAtomsPerParticle, float atomDiameter)
{
    m_volume = volume;
    m_collisionCoefficient = numberOfAtomsPerParticle*atomDiameter / (2*volume);
}

void Cell::collideParticles(float &vxi, float &vyi, float &vxj, float &vyj, const float relativeVelocity, Random *random) {
    const float vcmx = 0.5*(vxi + vxj); // Center of mass velocity
    const float vcmy = 0.5*(vyi + vyj);

    const float cosTheta = 1.0 - 2.0*random->nextDouble();
    const float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    const float vrelx = relativeVelocity*sinTheta;
    const float vrely = relativeVelocity*cosTheta;

    vxi = vcmx + 0.5*vrelx;
    vyi = vcmy + 0.5*vrely;
    vxj = vcmx - 0.5*vrelx;
    vyj = vcmy - 0.5*vrely;
}

unsigned long Cell::collide(float dt, Particles *particles, Random *random) {
    // Compute how many collision candidates to perform
    float numberOfCollisionCandidates = m_collisionCoefficient*m_numberOfParticles*(m_numberOfParticles-1)*m_maxRelativeVelocity*dt + m_collisionRest;
    unsigned int numberOfCollisionCandidatesRounded = round(numberOfCollisionCandidates);
    m_collisionRest = numberOfCollisionCandidates - numberOfCollisionCandidatesRounded;
    float *vx = &particles->vx[0];
    float *vy = &particles->vy[0];

    for(unsigned int collision=0; collision<numberOfCollisionCandidatesRounded; collision++) {
        const unsigned int localParticleIndex1 = random->nextDouble()*m_numberOfParticles;
        const unsigned int localParticleIndex2 = (localParticleIndex1 + ( (unsigned int)(random->nextDouble()* (m_numberOfParticles - 1) ))) % m_numberOfParticles;
        const unsigned int i = m_particleIndices[localParticleIndex1];
        const unsigned int j = m_particleIndices[localParticleIndex2];
        const float dvx = vx[i] - vx[j];
        const float dvy = vy[i] - vy[j];
        const float relativeVelocity = sqrt(dvx*dvx + dvy*dvy);
        if(relativeVelocity > m_maxRelativeVelocity) {
            m_maxRelativeVelocity = relativeVelocity;
        }

        if(relativeVelocity > random->nextDouble()*m_maxRelativeVelocity) {
            m_numberOfCollisions++;
            collideParticles(vx[i], vy[i], vx[j], vy[j], relativeVelocity, random);
        }
    }

    return m_numberOfCollisions;
}

void Cell::addParticle(unsigned int particleIndex, unsigned int *particleIndexMap)
{
    m_particleIndices[m_numberOfParticles] = particleIndex;
    particleIndexMap[particleIndex] = m_numberOfParticles;
    m_numberOfParticles++;
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
    m_maxRelativeVelocity = 0;
    for(unsigned int i=0; i<m_numberOfParticles; i++) {
        for(unsigned int j=i+1; j<m_numberOfParticles; j++) {
            float dvx = particles->vx[i] - particles->vx[j];
            float dvy = particles->vy[i] - particles->vy[j];
            float dv = sqrt(dvx*dvx + dvy*dvy);
            m_maxRelativeVelocity = std::max(m_maxRelativeVelocity, dv);
        }
    }
}
