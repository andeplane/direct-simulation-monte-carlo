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
    float meanCollisionTime = 1.0 / (sqrt(2) * system->settings()->atomDiameter*system->settings()->density*averageVelocity(system));
    return meanCollisionTime;
}

float StatisticsSampler::averageVelocity(System *system)
{
    double averageVelocity = 0;
    system->for_each([&](float x, float y, float vx, float vy) {
        averageVelocity += sqrt(vx*vx + vy*vy);
    });

    averageVelocity /= system->numberOfParticles();
    return averageVelocity;
}

void StatisticsSampler::sampleKineticEnergy(System *system)
{
    m_kineticEnergy = 0;
    float constant = 0.5*system->settings()->mass*system->settings()->atomsPerParticle;

    system->for_each([&](float x, float y, float vx, float vy) {
        m_kineticEnergy += constant*(vx*vx + vy*vy);
    });
}

void StatisticsSampler::sampleTemperature(System *system)
{
    m_temperature = 2.0/(3*system->numberOfParticles())*m_kineticEnergy;
}
