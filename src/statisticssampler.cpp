#include "statisticssampler.h"
#include "system.h"
#include <cmath>

StatisticsSampler::StatisticsSampler() :
    m_kineticEnergy(0),
    m_temperature(0)
{
}

void StatisticsSampler::sample(System *system)
{
    sampleKineticEnergy(system);
    sampleTemperature(system);
}

float StatisticsSampler::meanCollisionTime(System *system)
{
    float meanCollisionTime = 1.0 / (sqrt(2) * 2.0*system->settings()->atomDiameter*system->settings()->density*averageVelocity(system));
    return meanCollisionTime;
}

float StatisticsSampler::averageVelocity(System *system)
{
    Particles *particles = system->particles();
    double averageVelocity = 0;
    for(unsigned int i=0; i<particles->numberOfParticles(); i++) {
        averageVelocity += sqrt(particles->velocitySquared(i));
    }

    averageVelocity /= particles->numberOfParticles();
    return averageVelocity;
}

void StatisticsSampler::sampleKineticEnergy(System *system)
{
    m_kineticEnergy = 0;
    Particles *particles = system->particles();
    float constant = 0.5*system->settings()->mass*system->settings()->atomsPerParticle;

    for(unsigned int i=0; i<particles->numberOfParticles(); i++) {
        m_kineticEnergy += constant*particles->velocitySquared(i);
    }
}

void StatisticsSampler::sampleTemperature(System *system)
{
    m_temperature = 2.0/(3*system->particles()->numberOfParticles())*m_kineticEnergy;
}
