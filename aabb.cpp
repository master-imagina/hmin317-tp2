#include "aabb.h"


AABoundingBox::AABoundingBox() :
    m_center(),
    m_xBounds(),
    m_yBounds(),
    m_zBounds()
{}

AABoundingBox::AABoundingBox(const std::vector<VertexData> &vertices) :
    m_center(),
    m_xBounds(),
    m_yBounds(),
    m_zBounds()
{
    processVertices(vertices);
}

std::pair<float, float> AABoundingBox::xBounds() const
{
    return m_xBounds;
}

std::pair<float, float> AABoundingBox::yBounds() const
{
    return m_yBounds;
}

std::pair<float, float> AABoundingBox::zBounds() const
{
    return m_zBounds;
}

QVector3D AABoundingBox::center() const
{
    return m_center;
}

void AABoundingBox::processVertices(const std::vector<VertexData> &vertices)
{
    if (vertices.empty()) {
        m_center = QVector3D();
        m_xBounds = std::make_pair(0.f, 0.f);
        m_yBounds = std::make_pair(0.f, 0.f);
        m_zBounds = std::make_pair(0.f, 0.f);

        return;
    }

    // Process width
    const auto minMaxX =
            std::minmax_element(vertices.begin(), vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.x() < b.position.x();
    });

    m_xBounds = std::make_pair(minMaxX.first->position.x(),
                               minMaxX.second->position.x());

    // Process heights
    const auto minMaxY =
            std::minmax_element(vertices.begin(), vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.y() < b.position.y();
    });

    m_yBounds = std::make_pair(minMaxY.first->position.y(),
                               minMaxY.second->position.y());

    // Process depth
    const auto minMaxZ =
            std::minmax_element(vertices.begin(), vertices.end(),
                                [] (const VertexData &a, const VertexData &b) {
        return a.position.z() < b.position.z();
    });

    m_zBounds = std::make_pair(minMaxZ.first->position.z(),
                               minMaxZ.second->position.z());

    // Compute center
    m_center = QVector3D((m_xBounds.first + m_xBounds.second) * 0.5,
                         (m_yBounds.first + m_yBounds.second) * 0.5,
                         (m_zBounds.first + m_zBounds.second) * 0.5);
}
