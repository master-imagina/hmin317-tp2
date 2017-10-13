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

    void drawCubeGeometry(QOpenGLShaderProgram *program);
    void drawPlaneGeometry(QOpenGLShaderProgram *program);

private:
    void initCubeGeometry();
    void initPlaneGeometry();
    float generateRand(int max);

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    int TailleImage;
};

#endif // GEOMETRYENGINE_H
