#include "cellmanager.h"
#include "settings.h"
#include "system.h"
#include "cpelapsedtimer.h"
#include "random.h"
#include <cassert>
#include <cmath>
CellManager::CellManager() :
    m_numberOfCellsX(0),
    m_numberOfCellsY(0),
    m_numberOfCollisions(0),
    m_isInitialized(false),
    m_system(0)
{

}

void CellManager::initialize(System *system)
{
    m_system = system;
    Settings *settings = system->settings();
    unsigned int numberOfCells = settings->numberOfCells();
    float volumePerCell = settings->volume() / numberOfCells;
    float cellLength = pow(volumePerCell, 1.0/2.0);
    m_numberOfCellsX = settings->systemSize[0] / cellLength;
    m_numberOfCellsY = settings->systemSize[1] / cellLength;
    numberOfCells = m_numberOfCellsX*m_numberOfCellsY;

    m_cells.resize(numberOfCells);
    std::cout << "Setting volumes on all " << numberOfCells << " cells..." << std::endl;
    for(Cell &cell : m_cells) {
        cell.setVolume(volumePerCell, settings->atomsPerParticle, settings->atomDiameter);
    }
    std::cout << "Putting all particles in cells..." << std::endl;
    putAllParticlesInCells();
    m_isInitialized = true;
}

void CellManager::putAllParticlesInCells()
{
    CPElapsedTimer::updateCells().start();
    vec3 oneOverSystemSize = m_system->size();
    float positionToCellIndexFactorX = 1.0/oneOverSystemSize[0] * m_numberOfCellsX;
    float positionToCellIndexFactorY = 1.0/oneOverSystemSize[1] * m_numberOfCellsY;

    Particles *particles = m_system->particles();
    const unsigned int numberOfParticles = m_system->particles()->numberOfParticles();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        const unsigned int cx = particles->x[i]*positionToCellIndexFactorX;
        const unsigned int cy = particles->y[i]*positionToCellIndexFactorY;
        const unsigned int cellIndex = index(cx,cy);
#ifdef DSMC_DEBUG
        if(cellIndex < 0 || cellIndex >= m_cells.size()) {
            std::cout << "Particle " << i << " with position " << particles->x[i] << ", " << particles->y[i] << " got cell indices " << cx << " " << cy << " with cell index " << cellIndex << " which is out of bounds." << std::endl;
            exit(1);
        }
#endif

        Cell &newCell = m_cells[cellIndex];
        newCell.addParticle(i, m_particleIndexMap);
        m_particleCellMap[i] = cellIndex;
    }

    for(Cell &cell : m_cells) {
        cell.updateMaxRelativeVelocity(particles);
    }

    CPElapsedTimer::updateCells().stop();
}

void CellManager::updateParticleCells()
{
    CPElapsedTimer::updateCells().start();
    vec3 oneOverSystemSize = m_system->size();
    float positionToCellIndexFactorX = 1.0/oneOverSystemSize[0] * m_numberOfCellsX;
    float positionToCellIndexFactorY = 1.0/oneOverSystemSize[1] * m_numberOfCellsY;

    Particles *particles = m_system->particles();
    const unsigned int numberOfParticles = m_system->particles()->numberOfParticles();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        const unsigned int cx = particles->x[i]*positionToCellIndexFactorX;
        const unsigned int cy = particles->y[i]*positionToCellIndexFactorY;
        const unsigned int newCellIndex = index(cx,cy);
        const unsigned int oldCellIndex = m_particleCellMap[i];
#ifdef DSMC_DEBUG
        if(newCellIndex < 0 || newCellIndex >= m_cells.size()) {
            std::cout << "Particle " << i << " with position " << particles->x[i] << ", " << particles->y[i] << " got cell indices " << cx << " " << cy << " with cell index " << newCellIndex << " which is out of bounds." << std::endl;
            exit(1);
        }
#endif
        if(newCellIndex != oldCellIndex) {
            Cell &oldCell = m_cells[oldCellIndex];
            Cell &newCell = m_cells[newCellIndex];
            oldCell.removeParticle(i, m_particleIndexMap);
            newCell.addParticle(i, m_particleIndexMap);
            m_particleCellMap[i] = newCellIndex;
        }
    }
    CPElapsedTimer::updateCells().stop();
}

void CellManager::collide(float dt, Random *random)
{
    CPElapsedTimer::collideParticles().start();
    m_numberOfCollisions = 0;
    const unsigned int numberOfCells = m_cells.size();
    for(unsigned int i=0; i<numberOfCells; i++) {
        Cell &cell = m_cells[i];
        m_numberOfCollisions += cell.collide(dt, m_system->particles(), random);
        random->refillRandomFloats();
    }
    CPElapsedTimer::collideParticles().stop();
}
