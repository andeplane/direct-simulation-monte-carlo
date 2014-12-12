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

#pragma once

#include "simulator.h"

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QQuickFramebufferObject>
#include <QThread>
#include <QMutex>
#include "dsmcrenderer.h"

class DSMC : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D systemSize READ systemSize )
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool previousStepCompleted READ previousStepCompleted NOTIFY previousStepCompletedChanged)
    Q_PROPERTY(bool showGeometry READ showGeometry WRITE setShowGeometry NOTIFY showGeometryChanged)
    Q_PROPERTY(bool showParticles READ showParticles WRITE setShowParticles NOTIFY showParticlesChanged)
    Q_PROPERTY(bool showScalarField READ showScalarField WRITE setShowScalarField NOTIFY showScalarFieldChanged)

public:
    DSMC();
    ~DSMC();

    DSMCRenderer* createRenderer() const;

    Q_INVOKABLE void step();

    QVector3D systemSize() const;

    bool running() const;
    bool previousStepCompleted() const;
    bool simulatorOutputDirty() const;

    std::vector<QVector3D> m_positions;
    ScalarFieldContainer   m_scalarField;

    bool showGeometry() const
    {
        return m_showGeometry;
    }

    bool showParticles() const
    {
        return m_showParticles;
    }

    bool showScalarField() const
    {
        return m_showScalarField;
    }

public slots:
    void setRunning(bool arg);
    void setPreviousStepCompleted(bool arg);
    void setSimulatorOutputDirty(bool arg);

    void setShowGeometry(bool arg)
    {
        if (m_showGeometry == arg)
            return;

        m_showGeometry = arg;
        emit showGeometryChanged(arg);
    }

    void toggleShowParticles() {
        setShowParticles(!m_showParticles);
    }

    void toggleShowGeometry() {
        setShowGeometry(!m_showGeometry);
    }

    void toggleShowScalarField() {
        setShowScalarField(!m_showScalarField);
    }

    void setShowParticles(bool arg)
    {
        if (m_showParticles == arg)
            return;

        m_showParticles = arg;
        emit showParticlesChanged(arg);
    }

    void setShowScalarField(bool arg)
    {
        if (m_showScalarField == arg)
            return;

        m_showScalarField = arg;
        emit showScalarFieldChanged(arg);
    }

private slots:
    void finalizeStep();

signals:
    void runningChanged(bool arg);
    void previousStepCompletedChanged(bool arg);
    void requestStep();

    void showGeometryChanged(bool arg);

    void showParticlesChanged(bool arg);

    void showScalarFieldChanged(bool arg);

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:

    DSMCRenderer *m_renderer;
    Simulator m_dsmcSimulator;
    QVector3D m_systemSize;
    QElapsedTimer m_timer;
    bool m_running;

    bool m_stepRequested;
    bool m_previousStepCompleted;

    bool m_simulatorOutputDirty;
    bool m_lastStepWasBlocked;

    QMutex m_simulatorInputMutex;
    QMutex m_simulatorOutputMutex;
    QMutex m_simulatorRunningMutex;
    QThread m_simulatorWorker;

    friend class DSMCRenderer;
    void updatePositions();
    void updateScalarValues();
    bool m_showGeometry;
    bool m_showParticles;
    bool m_showScalarField;
};
