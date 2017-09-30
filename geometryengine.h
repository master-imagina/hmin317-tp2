#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void loadTerrainData(const std::vector<int> heights);
    void drawTerrainGeometry(QOpenGLShaderProgram *program);

private:
    QOpenGLBuffer m_arrayVbo;
    QOpenGLBuffer m_indexVbo;

    std::size_t m_terrainSize = 16;
    float m_maxHeight = 4.f;
};

#endif // GEOMETRYENGINE_H
