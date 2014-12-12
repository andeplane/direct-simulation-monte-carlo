#pragma once

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QQuickFramebufferObject>
#include <QThread>
#include <QMutex>
#include "scalarfield.h"
#include "points.h"

class DSMCRenderer : public QQuickFramebufferObject::Renderer
{
public:
    DSMCRenderer();
    ~DSMCRenderer();

    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void resetProjection();
    void setModelViewMatrices(double zoom, double tilt, double pan, double roll, const QVector3D &systemSize);

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void synchronize(QQuickFramebufferObject *item);
    void render();

private:
    QSize m_viewportSize;

    QVector3D m_systemSize;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QMatrix4x4 m_lightModelViewMatrix;

    std::vector<QVector3D> m_positions;
    Points *m_points;
    ScalarField *m_scalarField;

    bool m_skipNextFrame;
    bool m_showGeometry;
    bool m_showScalarField;
    bool m_showParticles;

    int m_syncCount;
    int m_renderCount;
    int m_dirtyCount;
    int m_atomCount;
};
