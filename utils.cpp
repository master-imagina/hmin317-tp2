#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <iostream>
#include "utils.h"
#include <time.h>

Utils::Utils()
    :indexBuf(QOpenGLBuffer::IndexBuffer)
{

}

float Utils::toRadians(float angle){
    return angle * M_PI / 180.0 ;
}

QImage Utils::generateHeightMap()
{

    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();
    generateQuad();

    QOpenGLFramebufferObject  * captureFBO;

    QOpenGLFramebufferObjectFormat format;
    format.setSamples(4);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    captureFBO = new QOpenGLFramebufferObject (512, 512,format);


    heightGeneratorProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    heightGeneratorProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/simpleQuadVertex.glsl");


    // Compile fragment shader
    heightGeneratorProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/heightGeneratorFragment.glsl");


    // Link shader pipeline
    heightGeneratorProgram->link();


    // Bind shader pipeline for use
    heightGeneratorProgram->bind();




    captureFBO->bind();


    glViewport(0, 0, 512, 512);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
    captureFBO->release();


    delete heightGeneratorProgram;
    return captureFBO->toImage();
}


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

void Utils::generateQuad()
{

    int gridSize = 2;
    std::vector<VertexData> vertices;
    double step = 1.f / (gridSize-1);
    for(int i=0;i<gridSize;i++){
        for(int j=0;j<gridSize;j++){

            vertices.push_back({QVector3D((double)(i*step)*2.0 - 1.0,double(j*step)*2.0 - 1.0,0.0f),QVector2D(0,0)});
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

void Utils::renderQuad()
{
    srand (time(NULL));
    arrayBuf.bind();

    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = heightGeneratorProgram->attributeLocation("a_position");
    heightGeneratorProgram->enableAttributeArray(vertexLocation);
    heightGeneratorProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    int texcoordLocation = heightGeneratorProgram->attributeLocation("a_texcoord");
    heightGeneratorProgram->enableAttributeArray(texcoordLocation);
    heightGeneratorProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    heightGeneratorProgram->setUniformValue("r1", (float)(rand() % 100)/100.0f);
    heightGeneratorProgram->setUniformValue("r2", -(float)(rand() % 100)/100.0f);
    heightGeneratorProgram->setUniformValue("r3", (float)(rand() % 100)/100.0f);
    heightGeneratorProgram->setUniformValue("r4", (float)(rand() % 100)/100.0f);

    glDrawElements(GL_TRIANGLES, m_nomberIndices, GL_UNSIGNED_SHORT, 0);

    arrayBuf.destroy();
    indexBuf.destroy();
}

