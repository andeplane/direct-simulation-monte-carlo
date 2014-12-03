#include "cpelapsedtimer.h"
#include "particles.h"
#include "system.h"
#include "unitconverter.h"

#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

CPElapsedTimer::CPElapsedTimer()
{
    m_startedAt = clock();
    m_lastPing = 0;
}

void CPElapsedTimer::m_printReport(System *system)
{
    float totalTime = CPElapsedTimer::totalTime();
    float periodicBoundaryConditionsFraction = m_periodicBoundaryConditions.elapsedTime() / totalTime;
    float samplingFraction = m_sampling.elapsedTime() / totalTime;
    float diskFraction = m_disk.elapsedTime() / totalTime;
    float moveParticlesFraction = m_moveParticles.elapsedTime() / totalTime;
    float timeEvolutionFraction = m_timeEvolution.elapsedTime() / totalTime;
    float systemInitializeFraction = m_systemInitialize.elapsedTime() / totalTime;
    float updateCellsFraction = m_updateCells.elapsedTime() / totalTime;
    float collideParticlesFraction = m_collideParticles.elapsedTime() / totalTime;

    unsigned int numberOfTimesteps = system->numberOfTimesteps();
    unsigned int numberOfParticles = system->particles()->numberOfParticles();
    unsigned long numberOfCollisions = system->cellManager()->numberOfCollisions();

    cout << endl << "Program finished after " << CPElapsedTimer::totalTime() << " seconds. Time analysis:" << endl;
    cout << std::fixed
         << "    System initialize    : " << m_systemInitialize.elapsedTime() << " s ( " << 100*systemInitializeFraction << "%)" <<  endl
         << "    Time evolution       : " << m_timeEvolution.elapsedTime() << " s ( " << 100*timeEvolutionFraction << "%)" <<  endl
         << "    Move particles       : " << m_moveParticles.elapsedTime() << " s ( " << 100*moveParticlesFraction << "%)" <<  endl
         << "    Update cells         : " << m_updateCells.elapsedTime() << " s ( " << 100*updateCellsFraction << "%)" <<  endl
         << "    Collide particles    : " << m_collideParticles.elapsedTime() << " s ( " << 100*collideParticlesFraction << "%)" <<  endl
         << "    Periodic boundary    : " << m_periodicBoundaryConditions.elapsedTime() << " s ( " << 100*periodicBoundaryConditionsFraction << "%)" <<  endl
         << "    Disk                 : " << m_disk.elapsedTime() << " s ( " << 100*diskFraction << "%)" <<  endl
         << "    Sampling             : " << m_sampling.elapsedTime() << " s ( " << 100*samplingFraction << "%)" <<  endl;
    cout << endl <<  numberOfTimesteps / totalTime << " timesteps / second. " << endl;
    cout << numberOfParticles*numberOfTimesteps / (1000*totalTime) << "k particle-timesteps / second. " << endl;
    cout << numberOfCollisions/totalTime << " collisions / second (total " << numberOfCollisions << " collisions)." << endl;

    float totalTimePerDay = system->totalTime()/totalTime * 86400;
    float microSecondsPerDay = UC::timeToSI(totalTimePerDay)*1e6;
    cout << "Estimated " << microSecondsPerDay << " μs simulated time per day." << endl;
}

double CPElapsedTimer::ping()
{
    CPElapsedTimer &timer = CPElapsedTimer::getInstance();

    double deltaT = CPElapsedTimer::totalTime() - timer.m_lastPing;
    timer.m_lastPing = CPElapsedTimer::totalTime();
    return deltaT;
}
