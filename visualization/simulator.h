#pragma once

#include <QObject>

// #include <system.h>
// #include <settings.h>

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(QObject* parent = 0);
    ~Simulator();

    // System m_system;
    // Settings m_settings;
    // StatisticsSampler *m_sampler;

public slots:
    void step();

signals:
    void stepCompleted();
};
