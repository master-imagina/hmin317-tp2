#include "heightmap.h"

#include <cmath>

#include <QImage>


Geometry heightmapToGeometry(const QImage &heightmap)
{
    if (heightmap.isNull() || !heightmap.isGrayscale()) {
        return Geometry();
    }

    // Get heights
    const int width = heightmap.width();
    const int height = heightmap.height();

    std::vector<int> heights(width * height);

    int pxCounter = 0;
    std::generate(heights.begin(), heights.end(),
                  [&pxCounter, &width, &heightmap] {
        const int x = pxCounter / width;
        const int y = pxCounter % width;

        pxCounter++;

        return qGray(heightmap.pixel(x, y));
    });

    // Build geometry
    const int terrainSize = std::sqrt(heights.size());

    Geometry ret;

    // Helpers for computing vertices
    const std::size_t vertexCount = terrainSize * terrainSize;
    ret.vertices.resize(vertexCount);

    // Helpers for computing indices
    const std::size_t maxXZCoord = terrainSize - 1;

    const std::size_t indexCount = 3 * 2 * std::pow(maxXZCoord, 2);
    ret.indices.resize(indexCount);

    int indexCounter = 0;

    // Compute...
    for (int z = 0; z < terrainSize; z++) {
        for (int x = 0; x < terrainSize; x++) {
            // ...vertices
            const int vertexIndex = z * terrainSize + x;

            const float xCoord = vertexIndex % terrainSize;
            const float zCoord = std::floor(vertexIndex / static_cast<float>(terrainSize));

            const float vertexHeight = heights[vertexIndex];

            VertexData &vertex = ret.vertices.at(vertexIndex);
            vertex.position = {xCoord, vertexHeight, zCoord};
            vertex.texCoord = {xCoord / 3.f, zCoord / 2.f};

            // ... and indices
            if (x == maxXZCoord || z == maxXZCoord) {
                continue;
            }

            const unsigned int firstSharedIndex = (z + 1) * terrainSize + x;
            const unsigned int secondSharedIndex = z * terrainSize + (x + 1);

            ret.indices[indexCounter++] = vertexIndex;
            ret.indices[indexCounter++] = firstSharedIndex;
            ret.indices[indexCounter++] = secondSharedIndex;

            ret.indices[indexCounter++] = firstSharedIndex;
            ret.indices[indexCounter++] = (z + 1) * terrainSize + (x + 1);
            ret.indices[indexCounter++] = secondSharedIndex;
        }
    }

    return ret;
}
