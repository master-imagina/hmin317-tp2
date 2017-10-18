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
#include <iostream>

#include <math.h>

int MainWidget::keyPlusPressed =1;
int MainWidget::keyMinusPressed =0;


MainWidget::MainWidget(int msFramerate,int calendarOffset, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    sand(0),
    cliff(0),
    grass(0),
    rock(0),
    cliffNormal(0),
    sandNormal(0),
    grassNormal(0),
    rockNormal(0),
    sandDisp(0),
    grassDisp(0),
    rockDisp(0),
    cliffDisp(0),
    angularSpeed(0)
{
    mouseHaveBeenPress = false;
    time = new QTimer;
    connect(time,SIGNAL(timeout()),this,SLOT(update()));
    time->start(msFramerate);
    elapse = new QTime;
    elapse->start();
    keyZPressed=0,keySPressed=0,keyQPressed=0,keyDPressed=0,keySpacePressed=0,keyMajPressed=0;
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
    dx_autoRotate = 0;
    paused = 0;
    elapsedTime.start();
    this->calendarOffset = calendarOffset;
    calendar = 0;
    GLHaveBeenInitialized = false;
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();

    delete texture;
    delete sand;
    delete cliff;
    delete grass;
    delete rock;
    delete geometries;
    delete grassNormal;
    delete cliffNormal;
    delete sandNormal;
    delete rockNormal;
    delete grassDisp;
    delete cliffDisp;
    delete sandDisp;
    delete rockDisp;
    particulesSystem.cleanUp();
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());

    mouseHaveBeenPress = true;
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    /*QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;*/

    mouseHaveBeenPress = false;
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(mouseHaveBeenPress){
        this->dx = dx;
        this->dy = dy;
    }else{
        this->dx = 0;
        this->dy = 0;
    }
    anchor = event->pos();
}

void MainWidget::wheelEvent(QWheelEvent *event)
{
    wheelDelta = event->delta()/120;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Z){
        keyZPressed=1;
    }
    if(event->key() == Qt::Key_S){
        keySPressed=1;
    }
    if(event->key() == Qt::Key_Q){
        keyQPressed=1;
    }
    if(event->key() == Qt::Key_D){
        keyDPressed=1;
    }
    if(event->key() == Qt::Key_Space){
        keySpacePressed = 1;
    }
    if(event->key() == Qt::Key_Shift){
        keyMajPressed=1;
    }

    if(event->key() == Qt::Key_Minus){
        keyMinusPressed=1;
    }

    if(event->key() == Qt::Key_Plus){
        keyPlusPressed=1;
    }

    if(event->key() == Qt::Key_P){
        paused = (paused +1)%2;

    }

}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z){
        keyZPressed=0;
    }
    if(event->key() == Qt::Key_S){
        keySPressed=0;
    }
    if(event->key() == Qt::Key_Q){
        keyQPressed=0;
    }
    if(event->key() == Qt::Key_D){
        keyDPressed=0;
    }
    if(event->key() == Qt::Key_Space){
        keySpacePressed = 0;
    }
    if(event->key() == Qt::Key_Shift){
        keyMajPressed = 0;
    }

    if(event->key() == Qt::Key_Minus){
        keyMinusPressed=0;
    }

    if(event->key() == Qt::Key_Plus){
        keyPlusPressed=0;
    }
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{

    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }

}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();



    Utils u;
    texture = new QOpenGLTexture(u.generateHeightMap());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    initTextures();
    initShaders();


    camera.move(0,0,0,0,0,0,0,0);
//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling

//! [2]

    geometries = new GeometryEngine;
    qreal aspect = qreal(this->size().width()) / qreal(this->size().height());

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;

    QMatrix4x4 projection;
    projection.perspective(fov, aspect, zNear, zFar);
    camera.setProjectionMatrix(projection);

    particulesSystem.initParticuleSystem();
    particlesRenderer.initParticuleRenderer();
    terrainEffect.initTerrainEffect();

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

    // Compile tessellation Control shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/tcsShader.glsl"))
        close();

    // Compile tessellation Evaluation shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/tesShader.glsl"))
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





    sand = new QOpenGLTexture(QImage(":/sand.png").mirrored());

    // Set nearest filtering mode for texture minification
    sand->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    sand->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    sand->setWrapMode(QOpenGLTexture::Repeat);
    sand->setAutoMipMapGenerationEnabled(true);

    rock = new QOpenGLTexture(QImage(":/rock.png").mirrored());

    // Set nearest filtering mode for texture minification
    rock->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    rock->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    rock->setWrapMode(QOpenGLTexture::Repeat);
    rock->setAutoMipMapGenerationEnabled(true);


    cliff = new QOpenGLTexture(QImage(":/cliff.png").mirrored());

    // Set nearest filtering mode for texture minification
    cliff->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    cliff->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    cliff->setWrapMode(QOpenGLTexture::Repeat);
    cliff->setAutoMipMapGenerationEnabled(true);

    grass = new QOpenGLTexture(QImage(":/grass.png").mirrored());

    // Set nearest filtering mode for texture minification
    grass->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    grass->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grass->setWrapMode(QOpenGLTexture::Repeat);
    grass->setAutoMipMapGenerationEnabled(true);

    cliffNormal = new QOpenGLTexture(QImage(":/Normal.png").mirrored());

    // Set nearest filtering mode for texture minification
    cliffNormal->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    cliffNormal->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    cliffNormal->setWrapMode(QOpenGLTexture::Repeat);
    cliffNormal->setAutoMipMapGenerationEnabled(true);

    sandNormal = new QOpenGLTexture(QImage(":/sandNormal.png").mirrored());

    // Set nearest filtering mode for texture minification
    sandNormal->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    sandNormal->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    sandNormal->setWrapMode(QOpenGLTexture::Repeat);
    sandNormal->setAutoMipMapGenerationEnabled(true);

    grassNormal = new QOpenGLTexture(QImage(":/grassNormal.png").mirrored());

    // Set nearest filtering mode for texture minification
    grassNormal->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    grassNormal->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grassNormal->setWrapMode(QOpenGLTexture::Repeat);
    grassNormal->setAutoMipMapGenerationEnabled(true);

    rockNormal = new QOpenGLTexture(QImage(":/rockNormal.png").mirrored());

    // Set nearest filtering mode for texture minification
    rockNormal->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    rockNormal->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    rockNormal->setWrapMode(QOpenGLTexture::Repeat);
    rockNormal->setAutoMipMapGenerationEnabled(true);

    sandDisp = new QOpenGLTexture(QImage(":/sandDisp.png").mirrored());

    // Set nearest filtering mode for texture minification
    sandDisp->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    sandDisp->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    sandDisp->setWrapMode(QOpenGLTexture::Repeat);
    sandDisp->setAutoMipMapGenerationEnabled(true);

    grassDisp = new QOpenGLTexture(QImage(":/grassDisp.png").mirrored());

    // Set nearest filtering mode for texture minification
    grassDisp->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    grassDisp->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grassDisp->setWrapMode(QOpenGLTexture::Repeat);
    grassDisp->setAutoMipMapGenerationEnabled(true);

    rockDisp = new QOpenGLTexture(QImage(":/rockDisp.png").mirrored());

    // Set nearest filtering mode for texture minification
    rockDisp->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    rockDisp->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    rockDisp->setWrapMode(QOpenGLTexture::Repeat);
    rockDisp->setAutoMipMapGenerationEnabled(true);

    cliffDisp = new QOpenGLTexture(QImage(":/cliffDisp.png").mirrored());

    // Set nearest filtering mode for texture minification
    cliffDisp->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    cliffDisp->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    cliffDisp->setWrapMode(QOpenGLTexture::Repeat);
    cliffDisp->setAutoMipMapGenerationEnabled(true);
}

void MainWidget::updateCalendar(int calendar)
{
    this->calendar = (calendar+calendarOffset)%360;
}

QVector3D MainWidget::QV3_Mix(float f, QVector3D a, QVector3D b){
    return b*f +  a*(1.0-f);
}

QVector3D MainWidget::seasonalSkybox()
{
    QVector3D winter(183/255.0,207/255.0,247/255.0);
    QVector3D spring(0/255.0,161/255.0,255/255.0);
    QVector3D summer(0/255.0,207/255.0,255/255.0);
    QVector3D fall(211/255.0,142/255.0,57/255.0);

    if(calendar<80){//Winter is
        return winter;
    }else if(calendar<100){
        return QV3_Mix((calendar-80.0)/20.0,winter,spring);
    }
    else if(calendar<170){//Spring
        return spring;
    }else if(calendar<190){
        return QV3_Mix((calendar-170.0)/20.0,spring,summer);
    }
    else if(calendar<260){//Summmer
        return summer;
    }else if(calendar<280){
        return QV3_Mix((calendar-260.0)/20.0,summer,fall);
    }
    else if (calendar<340){//autommn
        return fall;
    }else{
        return QV3_Mix((calendar-340)/20.0,fall,winter);
    }
}
//! [4]


//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;



    QMatrix4x4 projection;
    projection.perspective(fov, aspect, zNear, zFar);
    camera.setProjectionMatrix(projection);
}
//! [5]

void MainWidget::paintGL()
{

    this->makeCurrent();

    float snowFactor=0;
    if(calendar < 40){
        snowFactor = calendar/40.0;
    }else if (calendar <80){
        snowFactor =(80.0-calendar)/40.0;
    }

    QMatrix4x4 mvp = camera.getProjectionMatrix()*camera.getViewMatrix();
    if(calendar<170){
        particulesSystem.proccessTextureParticles(texture,snowFactor);
    }
    terrainEffect.proccessTerrainEffect(particulesSystem.getParticlesTexture(),texture,calendar);

    glViewport(0, 0, this->width(), this->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(calendar<170){
        if(particulesSystem.getParticlesTexture())
            particlesRenderer.renderParticles(particulesSystem.getParticlesTexture(),mvp,snowFactor);
    }
    program.bind();
    QVector3D colorSky = seasonalSkybox();
    glClearColor(colorSky.x(), colorSky.y(), colorSky.z(), 1);

    /*Incremente saison*/
    if(calendarOffset==0){
        calendar = (elapse->elapsed()*30/10000)%360;
        emit changedCalendar(calendar);
    }

    if(paused ){
        camera.move(dx,dy,wheelDelta,keyZPressed,keySPressed,keyQPressed,keyDPressed,keySpacePressed-keyMajPressed);
    }
    else{
        dx_autoRotate += 0.2f * (keyPlusPressed - keyMinusPressed) * (elapsedTime.elapsed()/1000.0f);
        camera.move(dx_autoRotate,dy,wheelDelta,keyZPressed,keySPressed,keyQPressed,keyDPressed,keySpacePressed-keyMajPressed);
    }
    elapsedTime.restart();
    dx=0,dy=0;
    wheelDelta = 0;
    // Clear color and depth buffer


    texture->bind(0);
    sand->bind(1);
    grass->bind(2);
    cliff->bind(3);
    rock->bind(4);
    cliffNormal->bind(5);
    sandNormal->bind(6);
    grassNormal->bind(7);
    rockNormal->bind(8);
    sandDisp->bind(9);
    grassDisp->bind(10);
    rockDisp->bind(11);
    cliffDisp->bind(12);
    if(terrainEffect.getSnowMap())
        terrainEffect.getSnowMap()->bind(13);
    if(particulesSystem.getParticlesTexture())
        particulesSystem.getParticlesTexture()->bind(14);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, 0.0);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", mvp  );
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);
    program.setUniformValue("grass", 2);
    program.setUniformValue("sand", 1);
    program.setUniformValue("rock", 4);
    program.setUniformValue("cliff",3);
    program.setUniformValue("cliffNormal",5);
    program.setUniformValue("sandNormal",6);
    program.setUniformValue("grassNormal",7);
    program.setUniformValue("rockNormal",8);
    program.setUniformValue("sandDisp",9);
    program.setUniformValue("grassDisp",10);
    program.setUniformValue("rockDisp",11);
    program.setUniformValue("cliffDisp",12);
    program.setUniformValue("snowMap",13);
    program.setUniformValue("particlesMap",14);

    program.setUniformValue("ambientColor",colorSky);
    program.setUniformValue("calendar",calendar);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);

}
