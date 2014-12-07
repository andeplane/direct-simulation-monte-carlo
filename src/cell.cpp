#include "cell.h"
#include "particles.h"
#include "random.h"
#include <cmath>
#include <algorithm>
#include <cassert>

Cell::Cell() :
    m_numberOfCollisionTrials(0),
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

void Cell::collideParticles(unsigned int i, unsigned int j, const float relativeVelocityHalf, const float randomNumber) {

    const float vcmx = 0.5f*(m_particles.vx[i] + m_particles.vx[j]); // Center of mass velocity
    const float vcmy = 0.5f*(m_particles.vy[i] + m_particles.vy[j]);

    const float cosTheta = 1.0f - 2.0f*randomNumber;
    const float sinTheta = sqrt(1.0f - cosTheta*cosTheta);

    const float vrelxHalf = relativeVelocityHalf*sinTheta;
    const float vrelyHalf = relativeVelocityHalf*cosTheta;

    m_particles.vx[i] = vcmx + vrelxHalf;
    m_particles.vy[i] = vcmy + vrelyHalf;
    m_particles.vx[j] = vcmx - vrelxHalf;
    m_particles.vy[j] = vcmy - vrelyHalf;
}

unsigned long Cell::collide(float dt, Random *random) {
    // Compute how many collision candidates to perform

    float numberOfCollisionTrials = m_collisionCoefficient*m_particles.numberOfParticles()*(m_particles.numberOfParticles()-1)*sqrt(m_maxRelativeVelocitySquared)*dt + m_collisionRest;
    unsigned int numberOfCollisionTrialsRounded = round(numberOfCollisionTrials);
    m_collisionRest = numberOfCollisionTrials - numberOfCollisionTrialsRounded;
    m_numberOfCollisionTrials += numberOfCollisionTrialsRounded;

    for(unsigned int collision=0; collision<numberOfCollisionTrialsRounded; collision++) {
        // Use the generateSSE4 function to generate the 4 random numbers we need in this collision. They are all unsigned ints, so we need to normalize with random->normalizationFactor.
        unsigned int rands[4];
        random->generateSSE4(rands);
        const unsigned int i = rands[0]*random->normalizationFactor*m_particles.numberOfParticles();
        const unsigned int j = (i + ( (unsigned int)(rands[1]*random->normalizationFactor* (m_particles.numberOfParticles() - 1) ))) % m_particles.numberOfParticles();

        const float dvx = m_particles.vx[i] - m_particles.vx[j];
        const float dvy = m_particles.vy[i] - m_particles.vy[j];
        const float relativeVelocitySquared = dvx*dvx + dvy*dvy;

        if(relativeVelocitySquared > m_maxRelativeVelocitySquared) {
            m_maxRelativeVelocitySquared = relativeVelocitySquared;
        }

        float randomNumber = rands[2]*random->normalizationFactor;

        if(relativeVelocitySquared > randomNumber*randomNumber*m_maxRelativeVelocitySquared) {
            const float relativeVelocityHalf = 0.5*sqrt(relativeVelocitySquared);
            m_numberOfCollisions++;
            collideParticles(i, j, relativeVelocityHalf, rands[3]*random->normalizationFactor);
        }
    }

    return m_numberOfCollisions;
}

unsigned int Cell::addParticle(float x, float y, float vx, float vy)
{
    return m_particles.addParticle(x,y,vx,vy);
}

void Cell::removeParticle(unsigned int particleIndex)
{
    m_particles.removeParticle(particleIndex);
}

void Cell::updateMaxRelativeVelocity() {
    m_maxRelativeVelocitySquared = 0;
    for(unsigned int i=0; i<m_particles.numberOfParticles(); i++) {
        for(unsigned int j=i+1; j<m_particles.numberOfParticles(); j++) {
            float dvx = m_particles.vx[i] - m_particles.vx[j];
            float dvy = m_particles.vy[i] - m_particles.vy[j];
            float dv = dvx*dvx + dvy*dvy;
            m_maxRelativeVelocitySquared = std::max(m_maxRelativeVelocitySquared, dv);
        }
    }
}
