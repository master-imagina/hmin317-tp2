#include "terraingeometry.h"

#include <algorithm>

#include <QtMath>


TerrainGeometry::TerrainGeometry() :
    m_vertices(),
    m_indices()
{}

TerrainGeometry::~TerrainGeometry()
{}

void TerrainGeometry::loadTerrainData(const std::vector<int> &heights)
{
    const int terrainSize = qSqrt(heights.size());

    // Helpers for computing vertices
    const std::size_t vertexCount = terrainSize * terrainSize;
    m_vertices.resize(vertexCount);

    // Helpers for computing indices
    const std::size_t maxXZCoord = terrainSize - 1;

    const std::size_t indexCount = 3 * 2 * qPow(maxXZCoord, 2);
    m_indices.resize(indexCount);

    int indexCounter = 0;

    // Compute...
    for (int z = 0; z < terrainSize; ++z) {
        for (int x = 0; x < terrainSize; ++x) {
            // ...vertices
            const int vertexIndex = z * terrainSize + x;

            const float xCoord = vertexIndex % terrainSize;
            const float zCoord = std::floor(vertexIndex / static_cast<float>(terrainSize));

            const bool vertexIsBoundary =
                    x < 2 || x > terrainSize - 3
                    || z < 2 || z > terrainSize - 3;

            const float vertexHeight = (vertexIsBoundary) ? 0.f
                                                          : heights[vertexIndex];

            VertexData &vertex = m_vertices.at(vertexIndex);
            vertex.position = {xCoord, vertexHeight, zCoord};
            vertex.texCoord = {xCoord / 3.f, zCoord / 2.f};

            // ... and indices
            if (x == maxXZCoord || z == maxXZCoord) {
                continue;
            }

            const unsigned int firstSharedIndex = (z + 1) * terrainSize + x;
            const unsigned int secondSharedIndex = z * terrainSize + (x + 1);

            m_indices[indexCounter++] = vertexIndex;
            m_indices[indexCounter++] = firstSharedIndex;
            m_indices[indexCounter++] = secondSharedIndex;

            m_indices[indexCounter++] = firstSharedIndex;
            m_indices[indexCounter++] = (z + 1) * terrainSize + (x + 1);
            m_indices[indexCounter++] = secondSharedIndex;
        }
    }
}

const std::vector<VertexData> &TerrainGeometry::vertices() const
{
    return m_vertices;
}

const std::vector<unsigned int> &TerrainGeometry::indices() const
{
    return m_indices;
}

std::pair<float, float> TerrainGeometry::widthBounds() const
{
    if (m_vertices.empty()) {
        return std::make_pair(0., 0.);
    }

    const auto minMaxHeights =
            std::minmax_element(m_vertices.begin(), m_vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.x() < b.position.x();
    });

    return std::make_pair(minMaxHeights.first->position.x(),
                          minMaxHeights.second->position.x());
}

std::pair<float, float> TerrainGeometry::heightBounds() const
{
    if (m_vertices.empty()) {
        return std::make_pair(0., 0.);
    }

    const auto minMaxHeights =
            std::minmax_element(m_vertices.begin(), m_vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.y() < b.position.y();
    });

    return std::make_pair(minMaxHeights.first->position.y(),
                          minMaxHeights.second->position.y());
}

std::pair<float, float> TerrainGeometry::depthBounds() const
{
    if (m_vertices.empty()) {
        return std::make_pair(0., 0.);
    }

    const auto minMaxHeights =
            std::minmax_element(m_vertices.begin(), m_vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.z() < b.position.z();
    });

    return std::make_pair(minMaxHeights.first->position.z(),
                          minMaxHeights.second->position.z());
}
