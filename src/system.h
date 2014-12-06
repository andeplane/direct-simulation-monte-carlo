#ifndef SYSTEM_H
#define SYSTEM_H
#include "vec3.h"
#include "particles.h"
#include "settings.h"
#include "particlemover.h"
#include "cellmanager.h"
#include <vector>
using std::vector;

class System
{
private:
    Particles m_particles;
    CellManager m_cellManager;
    ParticleMover m_particleMover;
    vec3 m_size;
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
    Particles *particles() { return &m_particles; }
    void step(double dt);
    float volume() { return m_size[0]*m_size[1]*m_size[2]; }
    Settings *settings() { return m_settings; }
    vec3 size() const;
    void setSize(const vec3 &size);
    unsigned int numberOfTimesteps() const;
    void setNumberOfTimesteps(unsigned int numberOfTimesteps);
    double totalTime() const;
    void setTotalTime(double totalTime);
    CellManager *cellManager() { return &m_cellManager; }
};

#endif // SYSTEM_H
