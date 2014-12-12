#include "system.h"
#include "random.h"
#include "unitconverter.h"
#include "cpelapsedtimer.h"
#include <cmath>

#include <cassert>
#include <iostream>
using std::cout; using std::endl;

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
    m_random = new Random();

    m_settings = &settings;
    setSize(settings.systemSize);
    cout << "Initializing grid..." << endl;
    m_grid.initialize(128, 128, this);
    m_grid.createSphere();

    cout << "Initializing particle mover..." << endl;
    m_particles = new Particles();
    m_particleMover.initialize(this);

    if(settings.loadState) {
        cout << "Loading state..." << endl;
        loadState();
    } else {
        createParticles();
    }

    cout << "Initializing cells..." << endl;
    m_cellManager.initialize(this);

    m_isInitialized = true;
    CPElapsedTimer::systemInitialize().stop();
    cout << "System initialized with size " << m_size << endl;
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
    unsigned int numberOfParticles = m_settings->numberOfParticles();
    m_particles->setNumberOfParticles(numberOfParticles);

    cout << "Creating " << numberOfParticles << " particles..." << endl;
    for(unsigned int i=0; i<numberOfParticles; i++) {
        m_particles->findPosition(i, m_size, m_random, &m_grid);
        m_particles->maxwellianVelocity(i, m_settings->temperature, m_settings->mass*m_settings->atomsPerParticle, m_random);
        m_random->refillRandomDoubles();
    }
}

vec2 System::size() const
{
    return m_size;
}

void System::setSize(const vec2 &size)
{
    m_size = size;
}

void System::step(double dt)
{
    CPElapsedTimer::timeEvolution().start();
    assert(m_isInitialized && "System is not initialized.");
    m_particleMover.moveParticles(dt);
    m_cellManager.updateParticleCells();
    m_cellManager.collide(dt, m_random);

    m_random->refillRandomDoubles();
    m_random->refillRandomFloats();
    m_random->refillRandomUnsignedInts();

    m_totalTime += dt;
    m_numberOfTimesteps++;
    if(m_numberOfTimesteps % m_settings->recomputeMaxRelativeVelocityEvery == 0) {
        m_cellManager.recomputeMaxRelativeVelocities();
    }
    CPElapsedTimer::timeEvolution().stop();
}
