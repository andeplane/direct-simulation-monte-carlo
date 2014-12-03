#pragma once

class System;
class ParticleMover
{
private:
    System *m_system;
    bool    m_isInitialized;
public:
    ParticleMover();
    void initialize(System *system);
    void moveParticles(float dt);
    void applyPeriodicBoundaryConditions();
};
