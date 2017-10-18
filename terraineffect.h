#ifndef TERRAINEFFECT_H
#define TERRAINEFFECT_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLExtraFunctions>

class TerrainEffect: protected QOpenGLFunctions
{
public:
    TerrainEffect();
    void initTerrainEffect();
    void cleanUp();

    void proccessTerrainEffect(QOpenGLTexture *particlesTexture, QOpenGLTexture *heightMap, int calendar);
    void generateQuad();
    void renderQuad(QOpenGLTexture *particlesTexture, QOpenGLTexture *heightMap,int calendar);

    QOpenGLTexture *getSnowMap();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    QOpenGLShaderProgram* particleProgram;

    QOpenGLTexture* snowMap;
    QOpenGLFramebufferObject  * captureFBO;
    int m_nomberIndices;
    QOpenGLFramebufferObjectFormat format;
};

#endif // TERRAINEFFECT_H
