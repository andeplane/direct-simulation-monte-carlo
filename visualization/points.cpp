#include "points.h"
#include <QOpenGLFunctions>
#include <QColor>
#include <QRgb>

using std::vector;

Points::Points() :
    m_funcs(0),
    m_program(0)
{
    generateVBOs();
}

Points::~Points()
{
    if(m_funcs) delete m_funcs;
    if(m_program) delete m_program;
}

void Points::generateVBOs()
{
    ensureInitialized();
    m_funcs->glGenBuffers(1, m_vboIds);
}

void Points::ensureInitialized()
{
    if(!m_funcs) m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
}

QVector3D Points::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

void Points::update(const vector<QVector3D> &positions)
{
    ensureInitialized();
    m_vertices = positions;

    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(QVector3D), &m_vertices[0], GL_STATIC_DRAW);
}

void Points::createShaderProgram() {
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute highp vec4 a_position;\n"
                                           "void main() {\n"
                                           "    gl_Position = a_position;\n"
                                           "}");

        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "void main() {\n"
                                           "    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                           "}");


        m_program->link();
    }
}

void Points::render()
{
    if(m_vertices.size() == 0) return;
    ensureInitialized();
    createShaderProgram();

    m_program->bind();

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_funcs->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);

    // Draw cube geometry using indices from VBO 1
    glDrawArrays(GL_POINTS, 0, m_vertices.size());

    m_program->disableAttributeArray(vertexLocation);

    m_program->release();
}
