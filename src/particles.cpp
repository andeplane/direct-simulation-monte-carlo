#include "particles.h"
#include "random.h"
#include <cmath>


unsigned int Particles::numberOfParticles() const
{
    return m_numberOfParticles;
}

void Particles::setNumberOfParticles(unsigned int numberOfParticles)
{
    assert(numberOfParticles <= MAXNUMPARTICLES && "Max number of particles exceeded. Increase MAXNUMPARTICLES.");
    m_numberOfParticles = numberOfParticles;
}

void Particles::maxwellianVelocity(unsigned int particleIndex, float temperature, float mass, Random *random)
{
    float sqrtTemperatureOverMass = sqrt(temperature / mass);

    vx[particleIndex] = random->nextGaussian(0, sqrtTemperatureOverMass);
    vy[particleIndex] = random->nextGaussian(0, sqrtTemperatureOverMass);
}

void Particles::findPosition(unsigned int particleIndex, vec2 systemSize, Random *random)
{
    x[particleIndex] = random->nextDouble() * systemSize[0];
    y[particleIndex] = random->nextDouble() * systemSize[1];
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
