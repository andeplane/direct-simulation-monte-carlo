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
    settings.systemSize = vec3(0.25, 0.25, 0.25);

    system.initialize(settings);
    float dt = 0.01;
    FileManager file;
    Particles *particles = system.particles();

    for(unsigned int timestep = 0; timestep < timesteps; timestep++) {
        system.step(dt);
        if(timestep % 100 == 0) {
            cout << timestep << endl;
        }

        file.writeXyz(particles);
    }

    CPElapsedTimer::printReport(&system);

    return 0;
}

