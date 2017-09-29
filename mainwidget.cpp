#include "mainwidget.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

#include <math.h>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0)
{
    fPositionX = 0.0;
    fPositionY = 0.0;
    fPositionZ = -10.0;
    for (int i=0;i<10;i++)
    {
        bKeys[i] = false;
    }
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

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    switch (key)
    {
        case Qt::Key_Z:
            e->accept();
            bKeys[0] = true;
            break;
        case Qt::Key_S:
            e->accept();
            bKeys[1] = true;
            break;
        case Qt::Key_Q:
            e->accept();
            bKeys[2] = true;
            break;
        case Qt::Key_D:
            e->accept();
            bKeys[3] = true;
            break;
        case Qt::Key_A:
            e->accept();
            bKeys[4] = true;
            break;
        case Qt::Key_E:
            e->accept();
            bKeys[5] = true;
            break;
        default:
            e->ignore();
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    switch (key)
    {
        case Qt::Key_Z:
            e->accept();
            bKeys[0] = false;
            break;
        case Qt::Key_S:
            e->accept();
            bKeys[1] = false;
            break;
        case Qt::Key_Q:
            e->accept();
            bKeys[2] = false;
            break;
        case Qt::Key_D:
            e->accept();
            bKeys[3] = false;
            break;
        case Qt::Key_A:
            e->accept();
            bKeys[4] = false;
            break;
        case Qt::Key_E:
            e->accept();
            bKeys[5] = false;
            break;
        default:
            e->ignore();
    }
}


void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    //QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    //qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    //rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    //angularSpeed += acc;
}



void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.98;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.02) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    }

    if (bKeys[0]) {fPositionY -= 0.1;fPositionZ += 0.1;}
    if (bKeys[1]) {fPositionY += 0.1;fPositionZ -= 0.1;}
    if (bKeys[2]) fPositionX += 0.1;
    if (bKeys[3]) fPositionX -= 0.1;
    if (bKeys[4]) fPositionZ += 0.1;
    if (bKeys[5]) fPositionZ -= 0.1;

    update();
}


void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();


    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    // glEnable(GL_CULL_FACE);






    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}


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



void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage("heightmap-3.png"));//.mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}



void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 3000.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    texture->bind();


    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.rotate(0.0f,0.0f,0.0f,0.0f);
    matrix.translate(fPositionX, fPositionY, fPositionZ);
    matrix.rotate(45.0f,-45.0f,0.0f,0.0f);



    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);


    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
