#include "mainwidget.h"

#include <math.h>

#include <QMatrix4x4>
#include <QtMath>

#include "camera.h"
#include "cameracontroller.h"


GameWidget::GameWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_timer(),
    m_shaderProgram(),
    m_geometryEngine(nullptr),
    m_texture(nullptr),
    m_camera(std::make_unique<Camera>()),
    m_cameraController(new CameraController(this))
{
    installEventFilter(m_cameraController);

    m_camera->setEyePos({8, 20, 8});
    m_camera->setTheta(4.7f);
}

GameWidget::~GameWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete m_texture;
    delete m_geometryEngine;
    doneCurrent();
}

void GameWidget::timerEvent(QTimerEvent *)
{
    update();
}

void GameWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_geometryEngine = new GeometryEngine;

    m_timer.start(12, this);
}

void GameWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 ":/geom_textured.vert")) {
        close();
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/geom_textured.frag")) {
        close();
    }

    if (!m_shaderProgram.link()) {
        close();
    }

    if (!m_shaderProgram.bind()) {
        close();
    }
}

void GameWidget::initTextures()
{
    m_texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void GameWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void GameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_texture->bind();

    // Calculate view transformation
    m_cameraController->updateCamera(m_camera.get());

    // Send uniforms to shaders
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();

    m_shaderProgram.setUniformValue("mvp_matrix",
                                    projectionMatrix * viewMatrix);
    m_shaderProgram.setUniformValue("texture", 0);

    // Draw geometry
    m_geometryEngine->drawTerrainGeometry(&m_shaderProgram);
}
