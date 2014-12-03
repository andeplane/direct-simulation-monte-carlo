#pragma once

#include <time.h>

class System;
class CPTimingObject {
private:
    double m_timeElapsed;
    clock_t m_startedAt;
public:
    CPTimingObject() : m_timeElapsed(0), m_startedAt(0) { }

    void start() {
         m_startedAt = clock();
    }

    double stop() {
        double t = double(clock() - m_startedAt)/CLOCKS_PER_SEC;
        m_timeElapsed += t;
        return t;
    }

    double elapsedTime() { return m_timeElapsed; }
};

class CPElapsedTimer
{
public:
    CPElapsedTimer();

    static CPElapsedTimer& getInstance()
    {
        static CPElapsedTimer instance; // Guaranteed to be destroyed.
                                 // Instantiated on first use.
        return instance;
    }

    clock_t        m_startedAt;
    clock_t        m_lastPing;
    CPTimingObject m_moveParticles;
    CPTimingObject m_periodicBoundaryConditions;
    CPTimingObject m_sampling;
    CPTimingObject m_disk;
    CPTimingObject m_timeEvolution;
    CPTimingObject m_systemInitialize;
    void m_printReport(System *system);

    static CPTimingObject &moveParticles() { return CPElapsedTimer::getInstance().m_moveParticles; }
    static CPTimingObject &periodicBoundaryConditions() { return CPElapsedTimer::getInstance().m_periodicBoundaryConditions; }
    static CPTimingObject &sampling() { return CPElapsedTimer::getInstance().m_sampling; }
    static CPTimingObject &disk() { return CPElapsedTimer::getInstance().m_disk; }
    static CPTimingObject &timeEvolution() { return CPElapsedTimer::getInstance().m_timeEvolution; }
    static CPTimingObject &systemInitialize() { return CPElapsedTimer::getInstance().m_systemInitialize; }
    static void printReport(System *system) { CPElapsedTimer::getInstance().m_printReport(system); }

    static double totalTime() { return double(clock() - CPElapsedTimer::getInstance().m_startedAt)/ CLOCKS_PER_SEC; }
    static double ping();
};
