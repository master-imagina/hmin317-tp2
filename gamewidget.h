#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class Camera;
class CameraController;
class TerrainGeometry;
class Renderer;


class GameWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void setGeometry(TerrainGeometry *geom);

    Camera *camera() const;

protected:
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void initShaders();
    void initTextures();

private:
    QBasicTimer m_timer;
    QOpenGLShaderProgram m_shaderProgram;

    TerrainGeometry *m_geometry;
    std::unique_ptr<Renderer> m_renderer;

    QOpenGLTexture *m_texture;

    std::unique_ptr<Camera> m_camera;
    CameraController *m_cameraController;
};

#endif // GAMEWIDGET_H
