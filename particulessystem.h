#ifndef PARTICULESSYSTEM_H
#define PARTICULESSYSTEM_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>

#define PARTICLE_MAX 512

class ParticulesSystem: protected QOpenGLFunctions
{
public:
    ParticulesSystem();

    void initParticuleSystem();
    void cleanUp();

    QImage proccessTextureParticles();
    void generateQuad();
    void renderQuad();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLShaderProgram* particleProgram;
    QOpenGLTexture* particuleTexture;

};

#endif // PARTICULESSYSTEM_H
