#include "dsmcrenderer.h"
#include "dsmc.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QGLFormat>
#include <QOpenGLContext>
#include <iostream>
#include <cmath>
#include <QOpenGLFramebufferObjectFormat>

using namespace std;

void DSMCRenderer::synchronize(QQuickFramebufferObject* item)
{
    m_syncCount++;
    DSMC *dsmc = (DSMC*)item; // TODO: Use correct casting method
    if(!dsmc) {
        return;
    }
    m_showGeometry = dsmc->showGeometry();
    m_showParticles = dsmc->showParticles();
    m_showScalarField = dsmc->showScalarField();
    resetProjection();

    if(dsmc->simulatorOutputDirty()) {
        dsmc->m_simulatorOutputMutex.lock();
        m_points->update(dsmc->m_positions);
        m_scalarField->update(dsmc->m_scalarField);
        m_lines->update(dsmc->m_lines);
        dsmc->setSimulatorOutputDirty(false);
        dsmc->m_simulatorOutputMutex.unlock();
        m_dirtyCount++;
    }
}

void DSMCRenderer::render()
{
    m_renderCount++;
    glDepthMask(true);

    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if(m_showScalarField) m_scalarField->render();
    if(m_showParticles) m_points->render();
    if(m_showGeometry) m_lines->render();

    glDepthMask(GL_TRUE);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


DSMCRenderer::DSMCRenderer() :
    m_skipNextFrame(false),
    m_showGeometry(true),
    m_showParticles(true),
    m_showScalarField(true),
    m_syncCount(0),
    m_renderCount(0),
    m_dirtyCount(0),
    m_atomCount(0)
{
    m_scalarField = new ScalarField();
    m_points = new Points();
    m_lines = new Lines();
}

DSMCRenderer::~DSMCRenderer()
{
    
}

void DSMCRenderer::resetProjection()
{
    // Calculate aspect ratio
    qreal aspect = qreal(m_viewportSize.width()) / qreal(m_viewportSize.height() ? m_viewportSize.height() : 1);
    
    // Set near plane to 3.0, far plane to 7.0, field of view 65 degrees
    const qreal zNear = 2.0, zFar = 2000.0, fov = 65.0;
    
    // Reset projection
    m_projectionMatrix.setToIdentity();
    
    // Set perspective projection
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
}

void DSMCRenderer::setModelViewMatrices(double zoom, double tilt, double pan, double roll, const QVector3D &systemSize)
{
    m_systemSize = systemSize;
    float systemSizeX = systemSize.x();
    float systemSizeY = systemSize.y();
    float systemSizeZ = systemSize.z();
    float systemSizeMax = sqrt(systemSizeX*systemSizeX + systemSizeY*systemSizeY + systemSizeZ*systemSizeZ);
    
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0,0,zoom);
    m_modelViewMatrix.rotate(90, 1, 0, 0);
    m_modelViewMatrix.rotate(tilt, 1, 0, 0);
    m_modelViewMatrix.rotate(pan, 0, 0, 1);
    m_modelViewMatrix.rotate(roll, 0, 1, 0);
    
    m_lightModelViewMatrix.setToIdentity();
    m_lightModelViewMatrix.translate(0,0,-systemSizeMax / 2.0);
    m_lightModelViewMatrix.rotate(90, 1, 0, 0);
    m_lightModelViewMatrix.rotate(tilt, 1, 0, 0);
    m_lightModelViewMatrix.rotate(pan, 0, 0, 1);
    m_lightModelViewMatrix.rotate(roll, 0, 1, 0);
}

QOpenGLFramebufferObject *DSMCRenderer::createFramebufferObject(const QSize &size) {
    m_viewportSize = size;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}
