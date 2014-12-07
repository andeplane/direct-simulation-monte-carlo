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

void CellManager::addParticle(float x, float y, float temperature, float mass, Random *random)
{
    const unsigned int cx = x/m_system->size()[0] * m_numberOfCellsX;
    const unsigned int cy = y/m_system->size()[1] * m_numberOfCellsY;
    const unsigned int cellIndex = index(cx,cy);
    Cell &cell = m_cells[cellIndex];
    unsigned int particleIndex = cell.addParticle(x,y);
    cell.particles()->maxwellianVelocity(particleIndex, temperature, mass, random);

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
    m_isInitialized = true;
}

void CellManager::updateParticleCells()
{
    CPElapsedTimer::updateCells().start();
    vec3 oneOverSystemSize = m_system->size();
    float positionToCellIndexFactorX = 1.0/oneOverSystemSize[0] * m_numberOfCellsX;
    float positionToCellIndexFactorY = 1.0/oneOverSystemSize[1] * m_numberOfCellsY;

    for(unsigned int cellIndex=0; cellIndex < m_cells.size(); cellIndex++) {
        Cell &cell = m_cells[cellIndex];
        Particles *particles = cell.particles();
        for(unsigned int i=0; i<particles->numberOfParticles(); i++) {
            const unsigned int cx = particles->x[i]*positionToCellIndexFactorX;
            const unsigned int cy = particles->y[i]*positionToCellIndexFactorY;
            const unsigned int newCellIndex = index(cx,cy);
#ifdef DSMC_DEBUG
            if(newCellIndex < 0 || newCellIndex >= m_cells.size()) {
                std::cout << "Particle " << i << " with position " << particles->x[i] << ", " << particles->y[i] << " got cell indices " << cx << " " << cy << " with cell index " << newCellIndex << " which is out of bounds." << std::endl;
                exit(1);
            }
#endif
            if(newCellIndex != cellIndex) {
                Cell &newCell = m_cells[newCellIndex];
                newCell.addParticle(particles->x[i], particles->y[i], particles->vx[i], particles->vy[i]);
                cell.removeParticle(i);
            }
        }
    }
    CPElapsedTimer::updateCells().stop();
}

void CellManager::recomputeRelativeVelocities()
{
    for(Cell &cell : m_cells) {
        cell.updateMaxRelativeVelocity();
    }
}

void CellManager::collide(float dt, Random *random)
{
    CPElapsedTimer::collideParticles().start();
    m_numberOfCollisions = 0;
    for(Cell &cell : m_cells) {
        m_numberOfCollisions += cell.collide(dt, random);
    }
    CPElapsedTimer::collideParticles().stop();
}

float CellManager::acceptanceRatio() {
    unsigned int totalCollisionTrials = 0;
    for(Cell &cell : m_cells) {
        totalCollisionTrials += cell.numberOfCollisionTrials();
    }

    return (double)m_numberOfCollisions / totalCollisionTrials;
}
