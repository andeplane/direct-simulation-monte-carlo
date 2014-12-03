#include <iostream>
#include <system.h>
#include <settings.h>
#include <unitconverter.h>
#include <filemanager.h>
#include <cpelapsedtimer.h>
#include <statisticssampler.h>

using namespace std;

int main()
{
    unsigned int timesteps = 1000;

    System system;
    Settings settings;
    // settings.systemSize = vec3(0.25, 0.25, 0.25);

    system.initialize(settings);
    FileManager file;
    Particles *particles = system.particles();
    StatisticsSampler sampler;
    float meanCollisionTime = sampler.meanCollisionTime(&system);
    cout << "Mean collision time: " << UC::timeToSI(meanCollisionTime)*1e9 << " ns." << endl;
    float dt = meanCollisionTime * 0.2;
    for(unsigned int timestep = 0; timestep < timesteps; timestep++) {
        system.step(dt);
        if(timestep % 100 == 0) {
            sampler.sample(&system);
            cout << timestep << "   E/n=" << UC::energyToEv(sampler.kineticEnergy())/system.particles()->numberOfParticles() << " [eV]   T=" << UC::temperatureToSI(sampler.temperature()) << " [K] " << endl;
        }

        // file.writeXyz(particles);
    }

    CPElapsedTimer::printReport(&system);

    return 0;
}

