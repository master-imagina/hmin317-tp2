#include "cameraactions.h"

#include <QQuaternion>

#include "camera.h"


void rotateCameraAroundTarget(Camera *camera, float angle)
{
    const QVector3D axis = camera->upVector();
    const QQuaternion quat = QQuaternion::fromAxisAndAngle(axis, angle);

    camera->setUpVector(quat * axis);
    camera->setEyePos(camera->targetPos() - quat * camera->viewVector());
}
