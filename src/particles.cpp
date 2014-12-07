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

void Particles::maxwellianVelocity(unsigned int particleIndex, float temperature, float mass, Random *random)
{
    float sqrtTemperatureOverMass = sqrt(temperature / mass);

    vx[particleIndex] = random->nextGaussian(0, sqrtTemperatureOverMass);
    vy[particleIndex] = random->nextGaussian(0, sqrtTemperatureOverMass);
}

void Particles::findPosition(unsigned int particleIndex, vec3 systemSize, Random *random)
{
    x[particleIndex] = random->nextDouble() * systemSize.x();
    y[particleIndex] = random->nextDouble() * systemSize.y();
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

unsigned int Particles::addParticle(float x_, float y_, float vx_, float vy_)
{
    x[m_numberOfParticles] = x_;
    y[m_numberOfParticles] = y_;
    vx[m_numberOfParticles] = vx_;
    vy[m_numberOfParticles] = vy_;
    m_numberOfParticles++;

    return m_numberOfParticles-1; // Return the index of the added particle
}

void Particles::removeParticle(unsigned int particleIndex)
{
    unsigned int lastParticleIndex = m_numberOfParticles-1;

    std::swap(x[particleIndex], x[lastParticleIndex]);
    std::swap(y[particleIndex], y[lastParticleIndex]);
    std::swap(vx[particleIndex], vx[lastParticleIndex]);
    std::swap(vy[particleIndex], vy[lastParticleIndex]);
    m_numberOfParticles--;
}
