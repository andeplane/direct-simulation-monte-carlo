#include "simulator.h"
#include <iostream>
#include <particles.h>
#include <statisticssampler.h>
#include <unitconverter.h>

using namespace std;

Simulator::Simulator(QObject *parent) :
    QObject(parent)
{
    system.initialize(settings);
    float meanCollisionTime = sampler.meanCollisionTime(&system);
    dt = meanCollisionTime * 0.2;
}

Simulator::~Simulator()
{

}

void Simulator::step() {
    system.step(dt);
    if(system.numberOfTimesteps() % 100 == 0) {
        sampler.sample(&system);
        cout << system.numberOfTimesteps() << "   E/n=" << UC::energyToEv(sampler.kineticEnergy())/system.particles()->numberOfParticles() << " [eV]   T=" << UC::temperatureToSI(sampler.temperature()) << " [K]   Acceptance ratio: " << system.cellManager()->acceptanceRatio() << endl;
    }
    emit stepCompleted();
}
