#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>

MainWidget::MainWidget(QWidget *parent, int frame) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0)
{
    Framerate = frame;

    PosX = -5.0;
    PosY = -15.0;
    PosZ = -15.0;
    for(int i = 0; i < 10; i++)
    {
        Pression[i] = false;
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

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis along the z axis
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    switch (key)
    {
        case Qt::Key_Z:
            Pression[0] = true;
            break;
        case Qt::Key_S:
            Pression[1] = true;
            break;
        case Qt::Key_Q:
            Pression[2] = true;
            break;
        case Qt::Key_D:
            Pression[3] = true;
            break;
        case Qt::Key_A:
            Pression[4] = true;
            break;
        case Qt::Key_E:
            Pression[5] = true;
            break;
        case Qt::Key_Minus:
            Pression[6] = true;
            break;
        case Qt::Key_Plus:
            Pression[7] = true;
            break;
        case Qt::Key_Escape:
            exit(1);
            break;
    }
    update();
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();

    switch (key)
    {
        case Qt::Key_Z:
            Pression[0] = false;
            break;
        case Qt::Key_S:
            Pression[1] = false;
            break;
        case Qt::Key_Q:
            Pression[2] = false;
            break;
        case Qt::Key_D:
            Pression[3] = false;
            break;
        case Qt::Key_A:
            Pression[4] = false;
            break;
        case Qt::Key_E:
            Pression[5] = false;
            break;
        case Qt::Key_Minus:
            Pression[6] = false;
            break;
        case Qt::Key_Plus:
            Pression[7] = false;
            break;
    }
    update();
}

void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    //angularSpeed *= 0.8;

    // Stop rotation when speed goes below threshold
   /* if (angularSpeed < 0.01)
        angularSpeed = 0.0;
    else
    {*/
        // Update rotation
        //rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), Vitesse) * rotation;
    //}

        if (Pression[0])
            PosY -= 0.1;

        if (Pression[1])
            PosY += 0.1;

        if (Pression[2])
            PosX += 0.1;

        if (Pression[3])
            PosX -= 0.1;

        if (Pression[4])
            PosZ += 0.1;

        if (Pression[5])
            PosZ -= 0.1;

        if (Pression[6])
        {
            if(Vitesse > 0)
                Vitesse -= 0.01;
        }

        if (Pression[7])
            Vitesse += 0.01;


        // Request an update
        update();
}
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

    geometries = new GeometryEngine();

    // Use QBasicTimer because its faster than QTimer
    timer.start(Framerate, this);
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
    texture = new QOpenGLTexture(QImage(":/heightmap-3.png"));

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
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;

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

    matrix.translate(PosX, 0, PosY);

    QQuaternion framing = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -45.0);
    matrix.rotate(framing);
    matrix.rotate(rotation);


    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
