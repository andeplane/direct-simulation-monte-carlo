#include "system.h"
#include "random.h"
#include "unitconverter.h"
#include "cpelapsedtimer.h"

#include <cassert>

System::System() :
    m_isInitialized(false),
    m_settings(0),
    m_numberOfTimesteps(0),
    m_totalTime(0)
{

}

void System::initialize(Settings &settings)
{
    CPElapsedTimer::systemInitialize().start();
    m_particleMover.initialize(this);
    m_settings = &settings;
    setSize(settings.systemSize);

    if(settings.loadState) {
        loadState();
    } else {
        createParticles();
    }

    m_isInitialized = true;
    CPElapsedTimer::systemInitialize().stop();
}


unsigned int System::numberOfTimesteps() const
{
    return m_numberOfTimesteps;
}

void System::setNumberOfTimesteps(unsigned int numberOfTimesteps)
{
    m_numberOfTimesteps = numberOfTimesteps;
}

double System::totalTime() const
{
    return m_totalTime;
}

void System::setTotalTime(double totalTime)
{
    m_totalTime = totalTime;
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

void System::step(double dt)
{
    CPElapsedTimer::timeEvolution().start();
    assert(m_isInitialized && "System is not initialized.");
    m_particleMover.moveParticles(dt);

    m_totalTime += dt;
    m_numberOfTimesteps++;
    CPElapsedTimer::timeEvolution().stop();
}
