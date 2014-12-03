#ifndef SYSTEM_H
#define SYSTEM_H
#include "vec3.h"
#include "particles.h"
#include "settings.h"
#include "particlemover.h"

class System
{
private:
    Particles m_particles;
    ParticleMover m_particleMover;
    vec3 m_size;
    bool m_isInitialized;
    Settings *m_settings;
    unsigned int m_numberOfTimesteps;
    double       m_totalTime;

    void loadState();
    void createParticles();
public:
    System();
    void initialize(Settings &settings);
    Particles &particles() { return m_particles; }
    void step(double dt);
    vec3 size() const;
    void setSize(const vec3 &size);
    float volume() { return m_size[0]*m_size[1]*m_size[2]; }
    unsigned int numberOfTimesteps() const;
    void setNumberOfTimesteps(unsigned int numberOfTimesteps);
    double totalTime() const;
    void setTotalTime(double totalTime);
};

#endif // SYSTEM_H
