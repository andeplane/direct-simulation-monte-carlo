#pragma once
#include "cell.h"
#include <vector>
using std::vector;

class System;
class CellManager
{
private:
    unsigned int m_particleCellMap[MAXNUMPARTICLES];
    unsigned int m_particleIndexMap[MAXNUMPARTICLES];
    unsigned int m_numberOfCellsX;
    unsigned int m_numberOfCellsY;
    unsigned long m_numberOfCollisions;
    vector<Cell> m_cells;
    bool m_isInitialized;
    System *m_system;
public:
    CellManager();
    void addParticle(float x, float y, float temperature, float mass, Random *random);
    unsigned int index(unsigned int cx, unsigned int cy) { return cx + cy*m_numberOfCellsX; }
    void initialize(System *system);
    void updateParticleCells();
    void recomputeRelativeVelocities();
    void collide(float dt, Random *random);
    unsigned long numberOfCollisions() { return m_numberOfCollisions; }
    float acceptanceRatio();
    vector<Cell> *cells() { return &m_cells; }
};

