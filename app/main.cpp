#include <iostream>
#include <system.h>
#include <settings.h>
#include <unitconverter.h>
#include <filemanager.h>
#include <cpelapsedtimer.h>

using namespace std;

int main()
{
    unsigned int timesteps = 1000;

    System system;
    Settings settings;
    settings.systemSize = vec3(0.5, 0.5, 0.5);

    system.initialize(settings);
    float dt = 0.01;
    FileManager file;
    Particles &particles = system.particles();

    for(unsigned int timestep = 0; timestep < timesteps; timestep++) {
        // cout << "Stepping timestep " << timestep << endl;
        system.step(dt);

        // file.writeXyz(&particles);
    }

    CPElapsedTimer::printReport(&system);

    return 0;
}

