#include "terraineffect.h"
#include <iostream>
#include <time.h>


TerrainEffect::TerrainEffect()
    :indexBuf(QOpenGLBuffer::IndexBuffer)
{

}

void TerrainEffect::initTerrainEffect()
{



    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();



    generateQuad();


    format.setSamples(2);
    format.setInternalTextureFormat(GL_RGB);
    captureFBO = new QOpenGLFramebufferObject (256, 256,format);


    snowMap = NULL;

    particleProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/simpleQuadVertex.glsl");


    // Compile fragment shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/terrainEffect.glsl");


    // Link shader pipeline
    particleProgram->link();


}

void TerrainEffect::cleanUp()
{

    arrayBuf.destroy();
    indexBuf.destroy();
    /*if(particuleTexture!=NULL)
        delete particuleTexture;
    if(extraDataTexture!=NULL)
        delete extraDataTexture;*/

}


void TerrainEffect::proccessTerrainEffect(QOpenGLTexture* particlesTexture,QOpenGLTexture* heightMap,int calendar)
{



    particleProgram->bind();



    captureFBO->bind();


    glViewport(0, 0, 256, 256);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad(particlesTexture,heightMap,calendar);
    captureFBO->release();

    if(snowMap!=NULL)
        delete snowMap;

    snowMap = new QOpenGLTexture(captureFBO->toImage(false,0));


    // Set nearest filtering mode for texture minification
    snowMap->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    snowMap->setMagnificationFilter(QOpenGLTexture::Nearest);



}


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

void TerrainEffect::generateQuad()
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

void TerrainEffect::renderQuad(QOpenGLTexture *particlesTexture, QOpenGLTexture *heightMap,int calendar)
{

    srand (time(NULL));
    arrayBuf.bind();

    indexBuf.bind();
    if(particlesTexture)
        particlesTexture->bind(0);
    if(snowMap)
        snowMap->bind(1);
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
    particleProgram->setUniformValue("particlesTex",0);
    particleProgram->setUniformValue("snowMap",1);
    particleProgram->setUniformValue("heightMap",2);
    particleProgram->setUniformValue("calendar",calendar);
    glDrawElements(GL_TRIANGLES, m_nomberIndices, GL_UNSIGNED_SHORT, 0);


}

QOpenGLTexture *TerrainEffect::getSnowMap()
{
    return snowMap;
}


