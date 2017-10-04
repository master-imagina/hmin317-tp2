#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

#include <QVector2D>
#include <QVector3D>


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

class Geometry
{
public:
    Geometry();
    ~Geometry();

    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
};

#endif // GEOMETRY_H
