#pragma once

class Grid;
class Particles;
class System;
class Random;

class ParticleMover
{
private:
    System    *m_system;
    Grid      *m_grid;
    Particles *m_particles;
    Random    *m_random;
    float   m_sqrtWallTemperatureOverMass;
    bool    m_isInitialized;
    void moveParticle(unsigned int particleIndex, double dt, unsigned int depth);
    void doMove(unsigned int particleIndex, double dt);
public:
    ParticleMover();
    void initialize(System *system);
    void moveParticles(double dt);
    void applyPeriodicBoundaryConditions();
};
