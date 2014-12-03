#include "particles.h"
#include "random.h"
#include <cmath>


unsigned int Particles::numberOfParticles() const
{
    return m_numberOfParticles;
}

void Particles::setNumberOfParticles(unsigned int numberOfParticles)
{
    m_numberOfParticles = numberOfParticles;
}

void Particles::maxwellianVelocity(unsigned int particleIndex, float temperature, float mass)
{
    float sqrtTemperatureOverMass = sqrt(temperature / mass);

    vx[particleIndex] = Random::nextGaussian(0, sqrtTemperatureOverMass);
    vy[particleIndex] = Random::nextGaussian(0, sqrtTemperatureOverMass);
}

void Particles::findPosition(unsigned int particleIndex, vec3 systemSize)
{
    x[particleIndex] = Random::nextDouble() * systemSize.x();
    y[particleIndex] = Random::nextDouble() * systemSize.y();
}

Particles::Particles() :
    m_numberOfParticles(0)
{

}
