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

#include "mainwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>

#include <math.h>
MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    fps(1000),
    speedController(1.01)

{
    eye.posX = 0;
    eye.posY = -70;
    eye.posZ = -150;
}

MainWidget::MainWidget(QWidget *parent,int fps) :
    QOpenGLWidget(parent),
    fps(fps),
    geometries(0),
    texture(0),
    angularSpeed(0),
    speedController(1.01)
{
    eye.posX = 0;
    eye.posY = -70;
    eye.posZ = -150;
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]



void MainWidget::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *c = dynamic_cast<QKeyEvent *>(event);
    if (c && c->key() ==  Qt::Key_Z){
        eye.forward=true;
    }

    if (c && c->key() ==  Qt::Key_Q){
        eye.left=true;


    }
    if (c && c->key() ==  Qt::Key_D){
        eye.right=true;

    }
    if (c && c->key() ==  Qt::Key_S){
        eye.backward=true;
    }
    if (c && c->key() ==  Qt::Key_Up){
        angularSpeed *=1.01;
    }
    if (c && c->key() ==  Qt::Key_Down){
        angularSpeed *=0.99;
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{

    QKeyEvent *c = dynamic_cast<QKeyEvent *>(event);
    if (c && c->key() ==  Qt::Key_Z){
        eye.forward=false;
    }

    if (c && c->key() ==  Qt::Key_Q){
        eye.left=false;
    }
    if (c && c->key() ==  Qt::Key_D){
        eye.right=false;

    }
    if (c && c->key() ==  Qt::Key_S){
        eye.backward=false;
    }
    if (c && c->key() ==  Qt::Key_Up){
        speedController *= 1.01;
    }
    if (c && c->key() ==  Qt::Key_Down){
        speedController *=0.99;
    }
}


//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{


    angularSpeed = pow(speedController, this->elapsedTimer.elapsed());

    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), 0.02 * this->elapsedTimer.elapsed() * angularSpeed) * rotation;
    this->elapsedTimer.restart();
    update();

    if (eye.forward) (eye.posZ += eye.speed),update();
    if (eye.right) (eye.posX -= eye.speed),update();
    if (eye.left) (eye.posX += eye.speed),update();
    if (eye.backward)  (eye.posZ-= eye.speed),update();
}


//! [1]
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    //! [2]

    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    int msec = 1000/fps;
    timer.start(msec, this);
    this->elapsedTimer.start();
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/heightmap-3.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 1.0, far plane to 10.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

    //! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;

    //matrix.translate(eye.posX,eye.posY,eye.posZ);

    //QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),-45.0);
    // matrix.rotate(framing);
    // matrix.translate(0.,1.8,0.);

    matrix.translate(-1.,0,0.); 

    QVector3D eye_vector = QVector3D(eye.posX,eye.posY,eye.posZ);
    QVector3D center = QVector3D(0.0,0.0,0.0);
    QVector3D up = QVector3D(0,1,0);

    matrix.lookAt(eye_vector,center,up);

    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
