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
      m_lastStepWasBlocked(false)
{
    m_timer.start();

    m_dsmcSimulator.moveToThread(&m_simulatorWorker);
    connect(this, &DSMC::requestStep, &m_dsmcSimulator, &Simulator::step);
    connect(&m_dsmcSimulator, &Simulator::stepCompleted, this, &DSMC::finalizeStep);
    m_simulatorWorker.start();
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

void DSMC::step()
{
    if(!m_running) {
        return;
    }

    if(m_simulatorRunningMutex.tryLock()) {
        double dt = m_timer.restart() / 1000.0;
        double safeDt = min(0.02, dt);
        // m_dsmcSimulator.m_system.setDt(safeDt);
        m_lastStepWasBlocked = false;
        emit requestStep();
    } else {
        m_lastStepWasBlocked = true;
    }
}

void DSMC::finalizeStep()
{
    m_simulatorOutputMutex.lock();
    // m_positions = m_dsmcSimulator.m_system.positions;
    m_previousStepCompleted = true;
    m_simulatorOutputDirty = true;
    m_simulatorOutputMutex.unlock();
    m_simulatorRunningMutex.unlock();
    update();

    if(m_lastStepWasBlocked && m_running) {
        m_lastStepWasBlocked = false;
        emit requestStep();
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
