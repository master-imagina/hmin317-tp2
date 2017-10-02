#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
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
    explicit GameWidget(unsigned int fps, QWidget *parent = 0);
    ~GameWidget();

    void setGeometry(TerrainGeometry *geom);
    void setRendererDirty();

    Camera *camera() const;
    void setCamera(Camera *camera);

protected:
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *e) override;

private:
    void initShaders();

private:
    unsigned int m_fps;
    QBasicTimer m_timer;

    QOpenGLShaderProgram m_shaderProgram;

    TerrainGeometry *m_geometry;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;
    CameraController *m_cameraController;
};

#endif // GAMEWIDGET_H
