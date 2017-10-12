#ifndef CAMERA_H
#define CAMERA_H

#include <QVector2D>
#include <QOpenGLWidget>
#include <QMatrix4x4>

typedef enum Camera_Movement {
    Z,
    S,
    Q,
    D,
    C,
    W
} Camera_Movement;

class Camera
{
public:
    Camera(QVector3D position = QVector3D(4.0f, 2.0f, 5.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = -45.0f)
        : cameraFront(0.0f, 0.0f, -1.0f)
    {
        cameraPos = position;
        worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }
    void processKeyPress(Camera_Movement movement);
    void processMouseMovement(float xoffset, float yoffset);
    void orbitAround();
    void lookAt(QMatrix4x4 &matrix);

    QVector3D getFront();

private:
    float pitch, yaw;
    QVector3D cameraPos, cameraFront, cameraUp, cameraRight, worldUp;

    void updateCameraVectors();
};

#endif // CAMERA_H
