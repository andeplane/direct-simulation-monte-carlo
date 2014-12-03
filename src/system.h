#ifndef SYSTEM_H
#define SYSTEM_H
#include "vec3.h"
#include "particles.h"
#include "settings.h"

class System
{
private:
    Particles m_particles;
    vec3 m_size;
    bool m_isInitialized;
    Settings *m_settings;

    void loadState();
    void createParticles();
public:
    System();
    void initialize(Settings &settings);
    Particles &getParticles() { return m_particles; }
    void step(float dt);
    vec3 size() const;
    void setSize(const vec3 &size);
    float volume() { return m_size[0]*m_size[1]*m_size[2]; }
};

#endif // SYSTEM_H
