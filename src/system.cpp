#include "system.h"
#include "random.h"
#include "unitconverter.h"
#include <cassert>

System::System() :
    m_isInitialized(false)
{

}

void System::initialize(Settings &settings)
{
    m_settings = &settings;
    setSize(settings.systemSize);

    if(settings.loadState) {
        loadState();
    } else {
        createParticles();
    }
}

void System::loadState()
{

}

void System::createParticles()
{
    unsigned int numberOfAtoms = m_settings->density * volume();
    unsigned int numberOfParticles = numberOfAtoms / m_settings->atomsPerParticle;

    m_particles.setNumberOfParticles(numberOfParticles);

    for(unsigned int i=0; i<numberOfParticles; i++) {
        m_particles.findPosition(i, m_size);
        m_particles.maxwellianVelocity(i, m_settings->temperature, m_settings->mass);
    }
}

vec3 System::size() const
{
    return m_size;
}

void System::setSize(const vec3 &size)
{
    m_size = size;
}

void System::step(float dt)
{
    assert(m_isInitialized && "System is not initialized.");
}
