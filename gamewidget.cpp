#include "gamewidget.h"

#include <QMatrix4x4>
#include <QPainter>
#include <QtMath>

#include "camera.h"
#include "renderer.h"
#include "terraingeometry.h"


GameWidget::GameWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_shaderProgram(),
    m_geometry(nullptr),
    m_renderer(nullptr),
    m_camera(nullptr)
{}

GameWidget::~GameWidget()
{
    // Make sure the context is current when freeing allocated resources
    makeCurrent();

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
    return m_camera;
}

void GameWidget::setCamera(Camera *camera)
{
    if (m_camera != camera) {
        m_camera = camera;
    }
}

void GameWidget::initializeGL()
{
    initializeOpenGLFunctions();

    m_renderer = std::make_unique<Renderer>();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
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

void GameWidget::resizeGL(int w, int h)
{
    m_camera->setAspectRatio(qreal(w) / qreal(h ? h : 1.f));
}

void GameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Send uniforms to shaders
    const QMatrix4x4 viewMatrix = m_camera->viewMatrix();
    const QMatrix4x4 projectionMatrix = m_camera->projectionMatrix();

    m_shaderProgram.setUniformValue("mvp_matrix",
                                    projectionMatrix * viewMatrix);

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
