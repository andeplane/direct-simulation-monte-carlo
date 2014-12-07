#include "particlemover.h"
#include "system.h"
#include "particles.h"
#include "cpelapsedtimer.h"

#include <cassert>

ParticleMover::ParticleMover() :
    m_system(0),
    m_isInitialized(false)
{

}

void ParticleMover::initialize(System *system)
{
    m_system = system;
    m_isInitialized = true;
}

void ParticleMover::moveParticles(float dt)
{
    assert(m_isInitialized && "ParticleMover not initialized.");
    CPElapsedTimer::moveParticles().start();
    Particles *particles = m_system->particles();
    const unsigned int numberOfParticles = particles->numberOfParticles();

    for(unsigned int i=0; i<numberOfParticles; i++) {
        particles->x[i] += particles->vx[i]*dt;
        particles->y[i] += particles->vy[i]*dt;
    }
    CPElapsedTimer::moveParticles().stop();

    applyPeriodicBoundaryConditions();
}

void ParticleMover::applyPeriodicBoundaryConditions()
{
    CPElapsedTimer::periodicBoundaryConditions().start();
    Particles *particles = m_system->particles();
    vec3 systemSize = m_system->size();

    const unsigned int numberOfParticles = particles->numberOfParticles();

    for(unsigned int i=0; i<numberOfParticles; i++) {
//        if(particles->x[i] < 0 && particles->x[i] > -1e-6) particles->x[i] = 0;
//        if(particles->y[i] < 0 && particles->y[i] > -1e-6) particles->y[i] = 0;

        if(particles->x[i] > systemSize[0]) particles->x[i] -= systemSize[0];
        else if(particles->x[i] < 0) particles->x[i] += systemSize[0];

        if(particles->y[i] > systemSize[1]) particles->y[i] -= systemSize[1];
        else if(particles->y[i] < 0) particles->y[i] += systemSize[1];
    }
    CPElapsedTimer::periodicBoundaryConditions().stop();
}
