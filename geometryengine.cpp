/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>

#include <QLabel>

#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initPlaneGeometry()
{
    float taille_max = 1.5;
    QImage img;

    if(!img.load(":/heightmap-1.png")) {
        std::cerr << "ERREUR CHARGEMENT FICHIER HEIGHTMAP." << std::endl;
        return;
    }

    int size = 256;

    int height = img.height(), width = img.width();
    int i_m = height / size, j_m = width / size;

    //std::cout << i_m << "," << j_m << std::endl;

    // Create array of 16 x 16 vertices facing the camera  (z=cte)
    VertexData vertices[size*size];

    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            {
                // Vertex data for face 0
                vertices[size*i+j] = { QVector3D(0.1*(i-(size / 2.0)),0.1*(j-(size / 2.0)), (float) qGray(img.pixel(i_m * i, j_m * j)) / 255.0 * taille_max + 1.5), QVector2D((float)i/size,(float)j/size)};
                // add height field eg (i-8)*(j-8)/256.0

                //std::cout << "Position pixel : " << i_m * i << "," << j_m * j << std::endl;
                //std::cout << "Valeur : " <<  (float) qGray(img.pixel(i_m * i, j_m * j)) / 255.0 * taille_max << std::endl;
        }


    // Draw 15 bands each with 32 vertices, with repeated vertices at the end of each band
    int nb_vertices = size * 2 + 4;
    taille_vertices = nb_vertices * (size - 1);
    GLushort indices[(size - 1) * nb_vertices];

    for (int i=0;i<size-1;i++)
        {
            indices[nb_vertices*i] = size*i;
            indices[nb_vertices*i+1] = size*i;

            for (int j=2;j<nb_vertices;j+=2)
                {
                    indices[nb_vertices*i+j] = size*i +(j-2)/2;
                    indices[nb_vertices*i+j+1] = size*(i+1) + (j-2)/2;
                }

            indices[nb_vertices*i+nb_vertices - 2] = size*(i+1) + size - 1;
            indices[nb_vertices*i+nb_vertices - 1] = size*(i+1) + size - 1;
    }

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, size*size * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, (size - 1) * nb_vertices * sizeof(GLushort));
//! [1]
}

//! [2]
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

    // Draw plane geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, taille_vertices, GL_UNSIGNED_SHORT, 0);
}
//! [2]
