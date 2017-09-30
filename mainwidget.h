#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <memory>

#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

#include "geometryengine.h"

class Camera;
class CameraController;
class GeometryEngine;


class GameWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

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
    GeometryEngine *m_geometryEngine;

    QOpenGLTexture *m_texture;

    std::unique_ptr<Camera> m_camera;
    CameraController *m_cameraController;
};

#endif // MAINWIDGET_H
