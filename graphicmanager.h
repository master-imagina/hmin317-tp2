#ifndef GRAPHICMANAGER_H
#define GRAPHICMANAGER_H

#include <iostream>
#include <math.h>

#include "terrain.h"
#include "global.h"
#include "drawing.h"
#include "cameraview.h"
#include "camerafreefly.h"
#include "inputmanager.h"

struct VBO_3D;
class CameraView;
class CameraFreeFly;
class Terrain;

class GraphicManager : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GraphicManager(QWidget *parent = 0);
    ~GraphicManager();

    static float window_width;
    static float window_height;

protected:
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void wheelEvent(QWheelEvent* e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();


private:

    int getElapsedTime();
    qint64 timer_prec;

    QElapsedTimer timer;
    QOpenGLShaderProgram program;

    QOpenGLTexture *texture;

    QVector2D mousePressPosition;

    int angle;
    float p_x;
    float p_y;
    VBO_3D vbo_plane;
    CameraView *cameraV;
    CameraFreeFly *cameraFF;

    Terrain *terrain;
};

#endif // GRAPHICMANAGER_H
