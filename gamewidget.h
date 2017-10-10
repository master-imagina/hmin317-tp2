#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <memory>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

#include "aabb.h"

class AABoundingBox;
class Camera;
class Geometry;
class Renderer;


class GameWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void setGeometry(Geometry *geom);
    void setRendererDirty();

    Camera *camera() const;
    void setCamera(Camera *camera);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void initShaders();

private:
    QOpenGLShaderProgram m_shaderProgram;

    Geometry *m_geometry;
    AABoundingBox m_terrainAABB;
    std::unique_ptr<Renderer> m_renderer;

    Camera *m_camera;
};

#endif // GAMEWIDGET_H
