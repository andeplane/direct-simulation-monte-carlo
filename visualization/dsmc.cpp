/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dsmc.h"
#include "dsmcrenderer.h"
#include <particles.h>
#include <system.h>

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QGLFormat>
#include <QOpenGLContext>
#include <iostream>
#include <cmath>
#include <QOpenGLFramebufferObjectFormat>
using namespace std;

DSMC::DSMC()
    :
      m_running(true),
      m_stepRequested(false),
      m_previousStepCompleted(true),
      m_simulatorOutputDirty(false),
      m_lastStepWasBlocked(false),
      m_showGeometry(true),
      m_showParticles(true),
      m_showScalarField(true)
{
    m_lines.updateId = 0;
    m_scalarField.numPointsX = 0;
    m_scalarField.numPointsY = 0;
    m_dsmcSimulator.moveToThread(&m_simulatorWorker);
    connect(this, &DSMC::requestStep, &m_dsmcSimulator, &Simulator::step);
    connect(&m_dsmcSimulator, &Simulator::stepCompleted, this, &DSMC::finalizeStep);
    m_simulatorWorker.start();
    m_timer.start();
}

DSMC::~DSMC()
{
    m_simulatorInputMutex.unlock();
    m_simulatorOutputMutex.unlock();
    m_simulatorRunningMutex.unlock();
    m_simulatorWorker.quit();
    m_simulatorWorker.wait();
}

DSMCRenderer *DSMC::createRenderer() const
{
    return new DSMCRenderer();
}


void DSMC::setRunning(bool arg)
{
    if (m_running == arg)
        return;
    
    m_running = arg;
    emit runningChanged(arg);
    update();
}

vec2 DSMC::scalePosition(const vec2 &position) {
    vec2 scaledPosition;
    scaledPosition[0] = 2*(position[0] / m_systemSize[0] - 0.5);
    scaledPosition[1] = 2*(position[1] / m_systemSize[1] - 0.5);
    return scaledPosition;
}

void DSMC::step()
{
    if(!m_running) {
        return;
    }
    if(m_simulatorRunningMutex.tryLock()) {
        // double dt = m_timer.restart() / 1000.0;
        m_lastStepWasBlocked = false;
        emit requestStep();
    } else {
        m_lastStepWasBlocked = true;
    }
}

void DSMC::updateLines()
{
    if(m_dsmcSimulator.system.geometry()->updateId() != m_lines.updateId) {
        Geometry *geometry = m_dsmcSimulator.system.geometry();
        m_lines.updateId = geometry->updateId();
        m_lines.pairs = geometry->lines();
        m_lines.points = geometry->points();
        for(vec2 &point : m_lines.points) {
            point = scalePosition(point);
        }
    }
}

void DSMC::updatePositions()
{
    Particles *particles = m_dsmcSimulator.system.particles();

    m_positions.resize(particles->numberOfParticles());
    const unsigned int numberOfParticles = particles->numberOfParticles();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        m_positions[i][0] = particles->x[i];
        m_positions[i][1] = particles->y[i];
        m_positions[i] = scalePosition(m_positions[i]);
    }
}

void DSMC::updateScalarValues()
{
    Grid *grid = m_dsmcSimulator.system.grid();
    //    m_scalarField.numPointsX = grid->width()+1;
    //    m_scalarField.numPointsY = grid->height()+1;
    m_scalarField.numPointsX = grid->width();
    m_scalarField.numPointsY = grid->height();

    unsigned int numVoxels = m_scalarField.numPointsX*m_scalarField.numPointsY;
    m_scalarField.values.resize(numVoxels);
    for(int i=0; i<int(m_scalarField.numPointsX); i++) {
        for(int j=0; j<int(m_scalarField.numPointsY); j++) {
            unsigned int thisGridIndex = i + j*m_scalarField.numPointsX;
            int index0 = grid->index(i,j);
            int index1 = grid->indexPeriodic(i-1, j);
            int index2 = grid->indexPeriodic(i-1, j-1);
            int index3 = grid->indexPeriodic(i, j-1);
            m_scalarField.values[thisGridIndex] = grid->voxelWithIndex(index0);
            // m_scalarField.values[thisGridIndex] = 0.25*(grid->voxelWithIndex(index0) + grid->voxelWithIndex(index1) + grid->voxelWithIndex(index2) + grid->voxelWithIndex(index3));
        }
    }
}

void DSMC::finalizeStep()
{

    m_simulatorOutputMutex.lock();
    m_systemSize[0] = m_dsmcSimulator.system.size()[0];
    m_systemSize[1] = m_dsmcSimulator.system.size()[1];
    m_systemSize[2] = 0;

    updatePositions();
    updateLines();
    updateScalarValues();
    m_previousStepCompleted = true;
    m_simulatorOutputDirty = true;
    m_simulatorOutputMutex.unlock();
    m_simulatorRunningMutex.unlock();
    update();
    
    if(m_lastStepWasBlocked) {
        m_lastStepWasBlocked = false;
        step();
    }
}

void DSMC::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        //        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

// ********************************************
// ******* Basic setters and getters **********
// ********************************************

void DSMC::setPreviousStepCompleted(bool arg)
{
    if (m_previousStepCompleted != arg) {
        m_previousStepCompleted = arg;
        emit previousStepCompletedChanged(arg);
    }
}

void DSMC::setSimulatorOutputDirty(bool arg)
{
    m_simulatorOutputDirty = arg;
}

QVector3D DSMC::systemSize() const
{
    return m_systemSize;
}

bool DSMC::running() const
{
    return m_running;
}

bool DSMC::previousStepCompleted() const
{
    return m_previousStepCompleted;
}

bool DSMC::simulatorOutputDirty() const
{
    return m_simulatorOutputDirty;
}
