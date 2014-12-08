#include <iostream>
#include <system.h>
#include <settings.h>
#include <unitconverter.h>
#include <filemanager.h>
#include <cpelapsedtimer.h>
#include <statisticssampler.h>
#include <cassert>

using namespace std;

int main(int args, char *argv[])
{
    unsigned int numTimesteps = 10000;
    System system;
    Settings settings;

    if(args > 1) {
        if(args < 7) {
            cout << "./app numTimesteps systemSize density temperature atomsPerParticle recomputeMaxRelativeVelocityEvery" << endl;
            cout << "./app 10000 1.0 1000000 3.0 10 100" << endl;
            exit(1);
        }

        numTimesteps = atoi(argv[1]);
        float systemSize = atof(argv[2]);
        float density = atof(argv[3]);
        float temperature = atof(argv[4]);
        unsigned int atomsPerParticle = atoi(argv[5]);
        unsigned int recomputeMaxRelativeVelocityEvery = atoi(argv[6]);

        settings.systemSize = vec2(systemSize, systemSize);
        settings.density = density;
        settings.temperature = temperature;
        settings.atomsPerParticle = atomsPerParticle;
        settings.recomputeMaxRelativeVelocityEvery = recomputeMaxRelativeVelocityEvery;
    }
    system.grid()->createBox();
    system.initialize(settings);
    FileManager file;
    Particles *particles = system.particles();
    StatisticsSampler sampler;
    float meanCollisionTime = sampler.meanCollisionTime(&system);
    cout << "Mean collision time: " << UC::timeToSI(meanCollisionTime)*1e9 << " ns." << endl;
    float dt = meanCollisionTime * 0.2;
    for(unsigned int timestep = 0; timestep < numTimesteps; timestep++) {
        system.step(dt);
        if(timestep % 1000 == 0) {
            sampler.sample(&system);
            cout << timestep << "   E/n=" << UC::energyToEv(sampler.kineticEnergy())/system.particles()->numberOfParticles() << " [eV]   T=" << UC::temperatureToSI(sampler.temperature()) << " [K]   Acceptance ratio: " << system.cellManager()->acceptanceRatio() << endl;
        }

        // file.writeXyz(particles);
    }

    CPElapsedTimer::printReport(&system);

    return 0;
}

