#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class QOpenGLShaderProgram;

class TerrainGeometry;


class Renderer : protected QOpenGLFunctions
{
public:
    Renderer();
    ~Renderer();

    void draw(TerrainGeometry *geom, QOpenGLShaderProgram *program);

    void cleanupResources();

private:
    QOpenGLBuffer m_arrayVbo;
    QOpenGLBuffer m_indexVbo;
};

#endif // RENDERER_H
