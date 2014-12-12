#ifndef SYSTEM_H
#define SYSTEM_H
#include "vec2.h"
#include "particles.h"
#include "settings.h"
#include "particlemover.h"
#include "cellmanager.h"
#include "grid.h"
#include "geometry.h"

#include <vector>
using std::vector;

class System
{
private:
    Particles *m_particles;
    Grid       m_grid;
    Geometry   m_geometry;
    CellManager m_cellManager;
    ParticleMover m_particleMover;
    vec2 m_size;
    bool m_isInitialized;
    Settings *m_settings;
    Random *m_random;
    unsigned int m_numberOfTimesteps;
    double       m_totalTime;

    void loadState();
    void createParticles();
    void createCells();
public:
    System();
    void initialize(Settings &settings);
    void step(double dt);
    float volume() { return m_size[0]*m_size[1]; }
    Settings *settings() { return m_settings; }
    vec2 size() const;
    void setSize(const vec2 &size);
    unsigned int numberOfTimesteps() const;
    void setNumberOfTimesteps(unsigned int numberOfTimesteps);
    double totalTime() const;
    void setTotalTime(double totalTime);
    Particles *particles() { return m_particles; }
    CellManager *cellManager() { return &m_cellManager; }
    Grid *grid() { return &m_grid; }
    Geometry *geometry() { return &m_geometry; }
    Random *random() { return m_random; }
};

#endif // SYSTEM_H
