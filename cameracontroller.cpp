#include "cameracontroller.h"

#include <QKeyEvent>

#include "camera.h"
#include "cameraactions.h"


CameraController::CameraController(QObject *parent) :
    QObject(parent),
    m_keyDirection(KeyDirection::None),
    m_isRotateAroundTargetPressed(false),
    m_rotateSpeed(1.f),
    m_moveSpeed(10.f),
    m_turboSpeed(m_moveSpeed * 4),
    m_turboKeyPressed(false)
{}

CameraController::~CameraController()
{}

bool CameraController::eventFilter(QObject *obj, QEvent *e)
{
    const QEvent::Type eventType = e->type();

    if (eventType == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyPressEvent(keyEvent);

        return false;
    }
    else if (eventType == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyReleaseEvent(keyEvent);

        return false;
    }

    return QObject::eventFilter(obj, e);
}

QVector3D CameraController::computeDirectionFromKeys(Camera *camera) const
{
    const QVector3D viewVec = camera->viewVector().normalized();
    const QVector3D rightVec = camera->rightVector();

    QVector3D ret;

    switch (m_keyDirection) {
    case KeyDirection::None:
        break;
    case KeyDirection::Up:
        ret = viewVec;
        break;
    case KeyDirection::Down:
        ret = viewVec * -1;
        break;
    case KeyDirection::Left:
        ret = rightVec * -1;
        break;
    case KeyDirection::Right:
        ret = rightVec;
        break;
    }

    return ret;
}

void CameraController::updateCamera(Camera *camera, unsigned int fps)
{
    const QVector3D oldEye = camera->eyePos();
    const QVector3D oldTarget = camera->targetPos();

    const float speed = (m_turboKeyPressed) ? m_turboSpeed : m_moveSpeed;
    const float factor = (fps / 1000.0f);
    const float moveAmount = factor * speed;

    const QVector3D moveDirection = computeDirectionFromKeys(camera);

    const QVector3D newEye = moveDirection * moveAmount + oldEye;
    const QVector3D newTarget = moveDirection * moveAmount + oldTarget;

    camera->setEyePos(newEye);
    camera->setTargetPos(newTarget);

    if (m_isRotateAroundTargetPressed) {
        const float angle = 2.f * factor * m_rotateSpeed;

        rotateCameraAroundTarget(camera, angle);
    }
}

void CameraController::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();

    KeyDirection newDirection = KeyDirection::None;

    if (key == Qt::Key_Z) {
        newDirection = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        newDirection = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        newDirection = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        newDirection = KeyDirection::Right;
    }

    else if  (key == Qt::Key_R) {
        m_isRotateAroundTargetPressed = true;
    }
    else if (key == Qt::Key_Up) {
        m_rotateSpeed *= 2.f;
    }
    else if (key == Qt::Key_Down) {
        m_rotateSpeed /= 2.f;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection | (int) newDirection);

    if (e->modifiers() & Qt::ShiftModifier) {
        m_turboKeyPressed = true;
    }
}

void CameraController::keyReleaseEvent(QKeyEvent *e)
{
    const int key = e->key();

    KeyDirection directionToExit = KeyDirection::None;

    if (key == Qt::Key_Z) {
        directionToExit = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        directionToExit = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        directionToExit = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        directionToExit = KeyDirection::Right;
    }

    else if  (key == Qt::Key_R) {
        m_isRotateAroundTargetPressed = false;
    }

    else {
        directionToExit = KeyDirection::None;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection & (~(int) directionToExit));

    if (key == Qt::Key_Shift) {
        m_turboKeyPressed = false;
    }
}
