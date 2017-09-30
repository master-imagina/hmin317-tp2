#include "geometryengine.h"

#include <cmath>

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GeometryEngine::GeometryEngine()
    : m_arrayVbo(),
      m_indexVbo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    m_arrayVbo.create();
    m_indexVbo.create();
}

GeometryEngine::~GeometryEngine()
{
    m_arrayVbo.destroy();
    m_indexVbo.destroy();
}

void GeometryEngine::loadTerrainData(const std::vector<int> heights)
{
    // Helpers for computing vertices
    const std::size_t vertexCount = m_terrainSize * m_terrainSize;
    std::vector<VertexData> vertices(vertexCount);

    // Helpers for computing indices
    const std::size_t maxXZCoord = m_terrainSize - 1;

    const std::size_t indexCount = 3 * 2 * std::pow(maxXZCoord, 2);
    std::vector<unsigned int> indices(indexCount);

    int indexCounter = 0;

    // Compute...
    for (int z = 0; z < m_terrainSize; ++z) {
        for (int x = 0; x < m_terrainSize; ++x) {
            // ...vertices
            const int vertexIndex = z * m_terrainSize + x;

            const float xCoord = vertexIndex % m_terrainSize;
            const float zCoord = std::floor(vertexIndex / static_cast<float>(m_terrainSize));

            const bool vertexIsBoundary =
                    x < 2 || x > m_terrainSize - 3
                    || z < 2 || z > m_terrainSize - 3;

            const float vertexHeight = (vertexIsBoundary) ? 0.f
                                                          : heights[vertexIndex];

            VertexData &vertex = vertices.at(vertexIndex);
            vertex.position = {xCoord, vertexHeight, zCoord};
            vertex.texCoord = {xCoord / 3.f, zCoord / 2.f};

            // ... and indices
            if (x == maxXZCoord || z == maxXZCoord) {
                continue;
            }

            const unsigned int firstSharedIndex = (z + 1) * m_terrainSize + x;
            const unsigned int secondSharedIndex = z * m_terrainSize + (x + 1);

            indices[indexCounter++] = vertexIndex;
            indices[indexCounter++] = firstSharedIndex;
            indices[indexCounter++] = secondSharedIndex;

            indices[indexCounter++] = firstSharedIndex;
            indices[indexCounter++] = (z + 1) * m_terrainSize + (x + 1);
            indices[indexCounter++] = secondSharedIndex;
        }
    }

    // VBOs
    m_arrayVbo.bind();
    m_arrayVbo.allocate(vertices.data(), vertexCount * sizeof(VertexData));

    m_indexVbo.bind();
    m_indexVbo.allocate(indices.data(), indexCount * sizeof(unsigned int));
}

void GeometryEngine::drawTerrainGeometry(QOpenGLShaderProgram *program)
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

    glDrawElements(GL_TRIANGLES, (m_terrainSize - 1) * (m_terrainSize - 1) * 6,
                   GL_UNSIGNED_INT, 0);
}
