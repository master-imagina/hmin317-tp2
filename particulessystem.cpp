#include "particulessystem.h"
#include <iostream>
#include <time.h>


ParticulesSystem::ParticulesSystem()
    :indexBuf(QOpenGLBuffer::IndexBuffer),extraDataTexture(QOpenGLTexture::Target2D)
{

}

void ParticulesSystem::initParticuleSystem()
{

    extraDataTexture.setFormat(QOpenGLTexture::RGBA16_UNorm);

    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();



    generateQuad();


    format.setSamples(2);
    format.setInternalTextureFormat(GL_RGB);
    captureFBO = new QOpenGLFramebufferObject (PARTICLE_MAX, PARTICLE_MAX,format);
    captureFBO->addColorAttachment(PARTICLE_MAX, PARTICLE_MAX);

    particuleTexture = NULL;

    particleProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/simpleQuadVertex.glsl");


    // Compile fragment shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particleEngine.glsl");


    // Link shader pipeline
    particleProgram->link();

    /*particleRenderProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    particleRenderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particleRenderVertex.glsl");



    // Link shader pipeline
    particleRenderProgram->link();*/

}

void ParticulesSystem::cleanUp()
{
    arrayBuf.destroy();
    indexBuf.destroy();
    //particlesArrayBuf.destroy();
}


void ParticulesSystem::proccessTextureParticles(QOpenGLTexture* heightMap)
{



    particleProgram->bind();



    captureFBO->bind();


    glViewport(0, 0, PARTICLE_MAX, PARTICLE_MAX);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    f->glDrawBuffers(2, bufs);
    renderQuad(heightMap);
    captureFBO->release();

    if(particuleTexture!=NULL)
        delete particuleTexture;

    particuleTexture = new QOpenGLTexture(captureFBO->toImage(false,0));


    // Set nearest filtering mode for texture minification
    particuleTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    particuleTexture->setMagnificationFilter(QOpenGLTexture::Nearest);

    extraDataTexture.destroy();
    extraDataTexture.setData(captureFBO->toImage(false,1));

    // Set nearest filtering mode for texture minification
    extraDataTexture.setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    extraDataTexture.setMagnificationFilter(QOpenGLTexture::Linear);

}


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

void ParticulesSystem::generateQuad()
{

    int gridSize = 2;
    std::vector<VertexData> vertices;
    double step = 1.f / (gridSize-1);
    for(int i=0;i<gridSize;i++){
        for(int j=0;j<gridSize;j++){

            vertices.push_back({QVector3D((double)(i*step)*2.0 - 1.0,double(j*step)*2.0 - 1.0,0.0f),QVector2D(i*step,j*step)});
        }
    }

    std::vector<GLushort> indices;
    for(int i=0;i<gridSize-1;i++){
        for(int j=0;j<gridSize-1;j++){
            int topleft = i  + j*gridSize;
            int topright = topleft + 1;
            int bottomleft = i + (j+1)*gridSize;
            int bottomright = bottomleft+1;


            indices.push_back(topleft);
            indices.push_back(bottomright);
            indices.push_back(topright);

            indices.push_back(topleft);
            indices.push_back(bottomleft);
            indices.push_back(bottomright);
        }
    }



    m_nomberIndices = indices.size();
    arrayBuf.bind();
    arrayBuf.allocate(&vertices[0], vertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.size() * sizeof(GLushort));



}

void ParticulesSystem::renderQuad(QOpenGLTexture *heightMap)
{

    srand (time(NULL));
    arrayBuf.bind();

    indexBuf.bind();
    if(particuleTexture)
        particuleTexture->bind(0);
    extraDataTexture.bind(1);
    heightMap->bind(2);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = particleProgram->attributeLocation("a_position");
    particleProgram->enableAttributeArray(vertexLocation);
    particleProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    int texcoordLocation = particleProgram->attributeLocation("a_texcoord");
    particleProgram->enableAttributeArray(texcoordLocation);
    particleProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    particleProgram->setUniformValue("resolution", QVector2D(PARTICLE_MAX,PARTICLE_MAX));
    particleProgram->setUniformValue("gravityVector",QVector3D(0,0.0000000981,0));
    particleProgram->setUniformValue("speed",1);
    particleProgram->setUniformValue("windVector",QVector3D(0,0,0));
    particleProgram->setUniformValue("particlesTex",0);
    particleProgram->setUniformValue("extraTex",1);
    particleProgram->setUniformValue("heightMap",2);
    particleProgram->setUniformValue("randomParameter", (float)(rand() % 100)/100.0f);
    glDrawElements(GL_TRIANGLES, m_nomberIndices, GL_UNSIGNED_SHORT, 0);


}

QOpenGLTexture *ParticulesSystem::getParticlesTexture()
{
    return particuleTexture;
}

QOpenGLTexture *ParticulesSystem::getExtraDataTexture()
{
    return &extraDataTexture;
}

