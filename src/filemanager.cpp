#include "filemanager.h"
#include "particles.h"
#include "cpelapsedtimer.h"

FileManager::FileManager()
{
}

void FileManager::writeXyz(Particles *particles)
{
    CPElapsedTimer::disk().start();
    if(!m_xyzFile.is_open()) {
        m_xyzFile.open("movie.xyz");
    }

    m_xyzFile << particles->numberOfParticles() << std::endl;
    m_xyzFile << "An awesome line." << std::endl;
    particles->for_each([&](float x, float y) {
        m_xyzFile << "Ar " << x << " " << y << " " << 0 << std::endl;
    });

    CPElapsedTimer::disk().stop();
}
