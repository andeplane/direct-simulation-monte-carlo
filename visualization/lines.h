#pragma once
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <vector>
#include <system.h>
#include <vector>
#include <utility>

struct LinesContainer {
    unsigned int updateId;
    vector<vec2> points;
    vector<std::pair<unsigned int, unsigned int> > pairs;
};

class Lines
{
public:
    Lines();
    ~Lines();
    void update(LinesContainer &lines);
    void render();
private:
    GLuint m_vboIds[2];
    std::vector<vec2> m_vertices;
    std::vector<unsigned int>    m_indices;
    QOpenGLFunctions *m_funcs;
    QOpenGLShaderProgram *m_program;

    void createShaderProgram();
    void generateVBOs();
    void ensureInitialized();
    QVector3D vectorFromColor(const QColor &color);
};
