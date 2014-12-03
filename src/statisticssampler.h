#ifndef STATISTICSSAMPLER_H
#define STATISTICSSAMPLER_H

class System;
class StatisticsSampler
{
private:
    float m_kineticEnergy;
    float m_temperature;
    void sampleKineticEnergy(System *system);
    void sampleTemperature(System *system);
public:
    StatisticsSampler();
    void sample(System *system);
    float kineticEnergy() { return m_kineticEnergy; }
    float temperature() { return m_temperature; }
    float averageVelocity(System *system);
    float meanCollisionTime(System *system);
};

#endif // STATISTICSSAMPLER_H
