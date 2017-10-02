#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>


class QKeyEvent;
class QMouseEvent;

class Camera;


class CameraController : public QObject
{
    Q_OBJECT

public:
    CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool eventFilter(QObject *obj, QEvent *e) override;

    void updateCamera(Camera *camera, unsigned int fps);

private:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    QVector3D computeDirectionFromKeys(Camera *camera) const;

private:
    enum KeyDirection
    {
        None = 0,
        Up = 1,
        Down = 2,
        Right = 4,
        Left = 8
    };

    KeyDirection m_keyDirection;

    bool m_isRotateAroundTargetPressed;
    float m_rotateSpeed;

    float m_moveSpeed;
    float m_turboSpeed;
    bool m_turboKeyPressed;
};

#endif // CAMERACONTROLLER_H
