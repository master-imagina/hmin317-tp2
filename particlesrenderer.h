#ifndef PARTICLESRENDERER_H
#define PARTICLESRENDERER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLExtraFunctions>

#include "particulessystem.h"

class ParticlesRenderer: protected QOpenGLFunctions_4_1_Core
{
public:
    ParticlesRenderer();

    void initParticuleRenderer();
    void cleanUp();

    void generateParticlesBuffer();
    void renderParticles(QOpenGLTexture* particlesTex, QMatrix4x4 mvp);

private:

    QOpenGLBuffer indexBuf;

    QOpenGLBuffer particlesArrayBuf;

    QOpenGLShaderProgram* particleRenderProgram;

    int m_nomberIndices;

};

#endif // PARTICLESRENDERER_H
