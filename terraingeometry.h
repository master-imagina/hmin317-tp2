#ifndef TERRAINGEOMETRY_H
#define TERRAINGEOMETRY_H

#include <cstddef>
#include <vector>

#include "geometry.h"


class TerrainGeometry
{
public:
    TerrainGeometry();
    ~TerrainGeometry();

    void loadTerrainData(const std::vector<int> &heights);

    const std::vector<VertexData> &vertices() const;
    const std::vector<unsigned int> &indices() const;

    std::pair<float, float> heightBounds() const;

    bool isDirty() const;
    void unsetDirty();

private:
    std::vector<VertexData> m_vertices;
    std::vector<unsigned int> m_indices;

    bool m_isDirty;
};

#endif // TERRAINGEOMETRY_H
