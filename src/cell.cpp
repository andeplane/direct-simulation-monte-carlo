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
    m_maxRelativeVelocitySquared(0)
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

    const float cosTheta = 1.0 - 2.0*random->nextFloat();
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
    float numberOfCollisionCandidates = m_collisionCoefficient*m_numberOfParticles*(m_numberOfParticles-1)*sqrt(m_maxRelativeVelocitySquared)*dt + m_collisionRest;
    unsigned int numberOfCollisionCandidatesRounded = round(numberOfCollisionCandidates);
    m_collisionRest = numberOfCollisionCandidates - numberOfCollisionCandidatesRounded;
    float *vx = &particles->vx[0];
    float *vy = &particles->vy[0];
    float maxRelativeVelocitySquared = m_maxRelativeVelocitySquared;
    for(unsigned int collision=0; collision<numberOfCollisionCandidatesRounded; collision++) {
        const unsigned int localParticleIndex1 = random->nextFloat()*m_numberOfParticles;
        const unsigned int localParticleIndex2 = (localParticleIndex1 + ( (unsigned int)(random->nextFloat()* (m_numberOfParticles - 1) ))) % m_numberOfParticles;
        // const unsigned int localParticleIndex1 = random->nextUnsignedInt(m_numberOfParticles-1);
        // const unsigned int localParticleIndex2 = (localParticleIndex1 + ( (unsigned int)(random->nextUnsignedInt(m_numberOfParticles - 2)))) % m_numberOfParticles;
        const unsigned int i = m_particleIndices[localParticleIndex1];
        const unsigned int j = m_particleIndices[localParticleIndex2];
        const float dvx = vx[i] - vx[j];
        const float dvy = vy[i] - vy[j];
        const float relativeVelocitySquared = dvx*dvx + dvy*dvy;
        if(relativeVelocitySquared > maxRelativeVelocitySquared) {
            maxRelativeVelocitySquared = relativeVelocitySquared;
        }

        float rnd = random->nextFloat();
        if(relativeVelocitySquared > rnd*rnd*maxRelativeVelocitySquared) {
            const float relativeVelocity = sqrt(relativeVelocitySquared);
            m_numberOfCollisions++;
            collideParticles(vx[i], vy[i], vx[j], vy[j], relativeVelocity, random);
        }
    }

    m_maxRelativeVelocitySquared = maxRelativeVelocitySquared;

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
    m_maxRelativeVelocitySquared = 0;
    for(unsigned int i=0; i<m_numberOfParticles; i++) {
        for(unsigned int j=i+1; j<m_numberOfParticles; j++) {
            float dvx = particles->vx[i] - particles->vx[j];
            float dvy = particles->vy[i] - particles->vy[j];
            float dv = dvx*dvx + dvy*dvy;
            m_maxRelativeVelocitySquared = std::max(m_maxRelativeVelocitySquared, dv);
        }
    }
}
