#ifndef UTILS_H
#define UTILS_H
#include <vector>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>

class Utils: protected QOpenGLFunctions
{
public:
    Utils();
    static float toRadians(float angle);
    QImage generateHeightMap();
    void generateQuad();
    void renderQuad();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLShaderProgram* heightGeneratorProgram;
    int m_nomberIndices;
};

#endif // UTILS_H
