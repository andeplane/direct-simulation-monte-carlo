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

void Particles::for_each(std::function<void (float, float, float, float)> action)
{
    for(unsigned int i=0; i<m_numberOfParticles; i++) {
        action(x[i], y[i], vx[i], vy[i]);
    }
}

void Particles::for_each(std::function<void (float, float)> action)
{
    for(unsigned int i=0; i<m_numberOfParticles; i++) {
        action(x[i], y[i]);
    }
}

Particles::Particles() :
    m_numberOfParticles(0)
{

}
