#pragma once

#include <QObject>

#include <system.h>
#include <settings.h>
#include <statisticssampler.h>

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(QObject* parent = 0);
    ~Simulator();

    System system;
    Settings settings;
    StatisticsSampler sampler;
    float dt;

public slots:
    void step();

signals:
    void stepCompleted();
};
