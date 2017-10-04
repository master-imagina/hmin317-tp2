#include "renderer.h"

#include <QOpenGLShaderProgram>

#include "geometry.h"
#include "geometry.h"


Renderer::Renderer() :
    m_arrayVbo(),
    m_indexVbo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    m_arrayVbo.create();
    m_indexVbo.create();
}

Renderer::~Renderer()
{
    cleanupResources();
}

void Renderer::updateBuffers(Geometry *geom)
{
    // Update VBOs
    const std::vector<VertexData> &vertices = geom->vertices;
    const std::vector<unsigned int> &indices = geom->indices;

    m_arrayVbo.bind();
    m_arrayVbo.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

    m_indexVbo.bind();
    m_indexVbo.allocate(indices.data(), indices.size() * sizeof(unsigned int));
}

void Renderer::draw(Geometry *geom, QOpenGLShaderProgram *program)
{
    m_arrayVbo.bind();
    m_indexVbo.bind();

    quintptr offset = 0;

    const int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    const int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, m_indexVbo.size() / 4, GL_UNSIGNED_INT, 0);
}

void Renderer::cleanupResources()
{
    if (m_arrayVbo.isCreated()) {
        m_arrayVbo.destroy();
    }

    if (m_indexVbo.isCreated()) {
        m_indexVbo.destroy();
    }
}
