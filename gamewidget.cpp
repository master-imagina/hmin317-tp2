#include "gamewidget.h"

#include <QApplication>
#include <QMatrix4x4>
#include <QtMath>

#include "camera.h"
#include "cameracontroller.h"
#include "renderer.h"
#include "terraingeometry.h"


GameWidget::GameWidget(unsigned int fps, QWidget *parent) :
    QOpenGLWidget(parent),
    m_fps(fps),
    m_timer(),
    m_shaderProgram(),
    m_geometry(nullptr),
    m_renderer(nullptr),
    m_texture(nullptr),
    m_camera(std::make_unique<Camera>()),
    m_cameraController(new CameraController(this))
{
    installEventFilter(m_cameraController);

    m_camera->setEyePos({8, 20, 8});
}

GameWidget::~GameWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();

    delete m_texture;
    m_renderer->cleanupResources();

    doneCurrent();
}

void GameWidget::setGeometry(TerrainGeometry *geom)
{
    if (m_geometry != geom) {
        m_geometry = geom;
    }
}

void GameWidget::setRendererDirty()
{
    m_renderer->updateBuffers(m_geometry);
}

Camera *GameWidget::camera() const
{
    return m_camera.get();
}

void GameWidget::timerEvent(QTimerEvent *)
{
    update();

    //FIXME Implement threaded rendering or avoid file dialogs instead of this
    qApp->processEvents();
}

void GameWidget::initializeGL()
{
    initializeOpenGLFunctions();

    m_renderer = std::make_unique<Renderer>();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_timer.start(1000 / m_fps, this);
}

void GameWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 "://res/shaders/geom_textured.vert")) {
        close();
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/res/shaders/terrain_heightmap.frag")) {
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

    // Compute camera position
    m_cameraController->updateCamera(m_camera.get(), m_fps);

    // Send uniforms to shaders
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();

    m_shaderProgram.setUniformValue("mvp_matrix",
                                    projectionMatrix * viewMatrix);
    m_shaderProgram.setUniformValue("texture", 0);

    const std::pair<float, float> heightBounds = m_geometry->heightBounds();
    m_shaderProgram.setUniformValue("minHeight", heightBounds.first);
    m_shaderProgram.setUniformValue("maxHeight", heightBounds.second);

    // Draw geometry
    m_renderer->draw(m_geometry, &m_shaderProgram);
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    setFocus();
}
