#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QRgb>
#include <cstdlib>
#include <iostream>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    iWidth = 256;
    iHeight = 256;

    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}


void GeometryEngine::initPlaneGeometry()
{
    QImage hmap;
    hmap.load("heightmap-3.png");

    iWidth = hmap.width()-1;
    iHeight = hmap.height()-1;
    iWidth = iHeight;






    float Heightmap[iWidth+1][iHeight+1];
    for (int i=0;i<iHeight+1;i++)
        for (int j=0;j<iWidth+1;j++)
        {
            QRgb px = hmap.pixel(i,j);
            Heightmap[i][j] = qBlue(px)/32.0f;
        }

    VertexData * vertices = (VertexData *)malloc(iWidth*iHeight*6*sizeof(VertexData));
    GLuint * indices = (GLuint *)malloc(iWidth*iHeight*6*sizeof(GLuint));

    for (float i=0;i<iHeight;i++)
    {
        for (float j=0;j<iWidth;j++)
        {
            for (int k=0;k<6;k++)
            {

                switch(k)
                {
                    case 0:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D(i*0.3f, j*0.3f,  Heightmap[(int)i][(int)j]),QVector2D(i/iWidth, j/iHeight)};
                        break;
                    case 1:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D((i+1)*0.3f, j*0.3f,  Heightmap[(int)i+1][(int)j]),QVector2D((i+1)/iWidth, j/iHeight)};
                        break;
                    case 2:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D(i*0.3f, (j+1)*0.3f,  Heightmap[(int)i][(int)j+1]),QVector2D(i/iWidth, (j+1)/iHeight)};
                        break;
                    case 3:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D((i+1)*0.3f, j*0.3f,  Heightmap[(int)i+1][(int)j]),QVector2D((i+1)/iWidth, j/iHeight)};
                        break;
                    case 4:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D((i+1)*0.3f, (j+1)*0.3f,  Heightmap[(int)i+1][(int)j+1]),QVector2D((i+1)/iWidth, (j+1)/iHeight)};
                        break;
                    case 5:
                        vertices[(int)(((i*iWidth)+j)*6+k)] = {QVector3D(i*0.3f, (j+1)*0.3f, Heightmap[(int)i][(int)j+1]),QVector2D(i/iWidth, (j+1)/iHeight)};
                        break;
                }
                indices[(int)(((i*iWidth)+j)*6+k)] = (GLuint)(((i*iWidth)+j)*6+k);


            }
        }
    }





    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, iWidth*iHeight* 6 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, iWidth*iHeight* 6 * sizeof(GLuint));



}


void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1




    glDrawElements(GL_TRIANGLES, iWidth*iHeight*6, GL_UNSIGNED_INT, 0);
}
