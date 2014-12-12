#pragma once
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <vector>
#include <system.h>

class Points
{
public:
    Points();
    ~Points();
    void update(const vector<vec2> &positions);
    void render();
private:
    GLuint m_vboIds[1];
    std::vector<vec2> m_vertices;
    QOpenGLFunctions *m_funcs;
    QOpenGLShaderProgram *m_program;

    void createShaderProgram();
    void generateVBOs();
    void ensureInitialized();
    QVector3D vectorFromColor(const QColor &color);
};
