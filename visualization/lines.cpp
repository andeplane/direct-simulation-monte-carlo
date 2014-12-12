#include "Lines.h"
#include <QOpenGLFunctions>
#include <QColor>
#include <QRgb>

using std::vector;

Lines::Lines() :
    m_funcs(0),
    m_program(0)
{
    generateVBOs();
}

Lines::~Lines()
{
    if(m_funcs) delete m_funcs;
    if(m_program) delete m_program;
}

void Lines::update(LinesContainer &lines)
{
    // TODO: Only update if dirty
    ensureInitialized();
    m_indices.clear();
    m_indices.reserve(2*lines.pairs.size());
    for(auto &pair : lines.pairs) {
        m_indices.push_back(pair.first);
        m_indices.push_back(pair.second);
    }

    m_vertices = lines.points;

    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec2), &m_vertices[0], GL_STATIC_DRAW);

    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
}

void Lines::generateVBOs()
{
    ensureInitialized();
    m_funcs->glGenBuffers(2, m_vboIds);
}

void Lines::ensureInitialized()
{
    if(!m_funcs) m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
}

QVector3D Lines::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

void Lines::createShaderProgram() {
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute highp vec2 a_position;\n"
                                           "void main() {\n"
                                           "    gl_Position = vec4(a_position.xy, 0.0, 1.0);\n"
                                           "}");

        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "void main() {\n"
                                           "    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                           "}");


        m_program->link();
    }
}

void Lines::render()
{
    if(m_vertices.size() == 0) return;
    ensureInitialized();
    createShaderProgram();

    m_program->bind();

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_funcs->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);

    m_program->disableAttributeArray(vertexLocation);

    m_program->release();
}
