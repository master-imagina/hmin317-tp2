#ifndef TERRAINGEOMETRY_H
#define TERRAINGEOMETRY_H

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

private:
    std::vector<VertexData> m_vertices;
    std::vector<unsigned int> m_indices;
};

#endif // TERRAINGEOMETRY_H
