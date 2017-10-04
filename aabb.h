#ifndef AABB_H
#define AABB_H

#include <vector>

#include "geometry.h"


class AABoundingBox
{
public:
    AABoundingBox();
    explicit AABoundingBox(const std::vector<VertexData> &vertices);

    std::pair<float, float> xBounds() const;
    std::pair<float, float> yBounds() const;
    std::pair<float, float> zBounds() const;

    QVector3D center() const;

    void processVertices(const std::vector<VertexData> &vertices);

private:
    std::pair<float, float> m_xBounds;
    std::pair<float, float> m_yBounds;
    std::pair<float, float> m_zBounds;

    QVector3D m_center;
};

#endif // AABB_H
