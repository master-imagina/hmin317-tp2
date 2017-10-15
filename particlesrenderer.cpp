#include "particlesrenderer.h"

ParticlesRenderer::ParticlesRenderer()
:indexBuf(QOpenGLBuffer::IndexBuffer)
{

}

void ParticlesRenderer::initParticuleRenderer()
{
    initializeOpenGLFunctions();


    particlesArrayBuf.create();
    indexBuf.create();
    generateParticlesBuffer();

    particleRenderProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particleRenderVertex.glsl");
    particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particleRenderFragment.glsl");


    // Link shader pipeline
    particleRenderProgram->link();
}

void ParticlesRenderer::generateParticlesBuffer()
{
    std::vector<QVector2D> index;
    std::vector<GLushort> indices;
    int k=0;
    for(int i=0;i<PARTICLE_MAX;i++){
        for(int j=0;j<PARTICLE_MAX;j++){

            index.push_back(QVector2D(i/(float)PARTICLE_MAX,j/(float)PARTICLE_MAX));
            indices.push_back(k++);
        }
    }

    particlesArrayBuf.bind();
    particlesArrayBuf.allocate(&index[0], index.size() * sizeof(QVector2D));
    m_nomberIndices = indices.size();
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.size() * sizeof(GLushort));
}

void ParticlesRenderer::renderParticles(QOpenGLTexture *particlesTex, QMatrix4x4 mvp)
{
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    glPointSize(10);
    particleRenderProgram->bind();
    particlesArrayBuf.bind();
    indexBuf.bind();
    particlesTex->bind(0);
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int indexLocation = particleRenderProgram->attributeLocation("index");
    particleRenderProgram->enableAttributeArray(indexLocation);
    particleRenderProgram->setAttributeBuffer(indexLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
    particleRenderProgram->setUniformValue("particlesTex",0);
    particleRenderProgram->setUniformValue("mvp",mvp);

    glDrawElements(GL_POINTS, m_nomberIndices, GL_UNSIGNED_SHORT, 0);
}


