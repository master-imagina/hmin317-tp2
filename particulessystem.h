#ifndef PARTICULESSYSTEM_H
#define PARTICULESSYSTEM_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLExtraFunctions>

#define PARTICLE_MAX 256

class ParticulesSystem: protected QOpenGLFunctions
{
public:
    ParticulesSystem();

    void initParticuleSystem();
    void cleanUp();

    void proccessTextureParticles(QOpenGLTexture *heightMap);
    void generateQuad();
    void renderQuad(QOpenGLTexture *heightMap);

    QOpenGLTexture* getParticlesTexture();
    QOpenGLTexture* getExtraDataTexture();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    //QOpenGLBuffer particlesArrayBuf;

    //QOpenGLShaderProgram* particleRenderProgram;
    QOpenGLShaderProgram* particleProgram;

    QOpenGLTexture* particuleTexture;
    QOpenGLTexture extraDataTexture;
    QOpenGLFramebufferObject  * captureFBO;
    int m_nomberIndices;
    QOpenGLFramebufferObjectFormat format;

};


#endif // PARTICULESSYSTEM_H
