#include "camera.h"

QVector3D Camera::position = QVector3D(0,0,0);
QVector3D Camera::target = QVector3D(0,0,0);
float Camera::angleHorizontal = 0;
float Camera::angleVertical = 0;

Camera::Camera()
{

}

void Camera::LookAt(float x, float y, float z, float tx,float ty, float tz)
{
    Draw::loadIdentity();

    position = QVector3D(-x,-y,-z);
    target = QVector3D(-tx,-ty,-tz);

    QVector3D direction = target - position;

    QVector2D vHorizontal(direction.z(),direction.x());
    angleHorizontal = -atan2(vHorizontal.y(),vHorizontal.x()) * 180.0 / M_PI;

    Draw::rotate(angleHorizontal,0,1,0);

    QVector2D vVertical(vHorizontal.length(),direction.y());
    angleVertical = atan2(vVertical.y(),vVertical.x()) * 180.0 / M_PI;

    Draw::rotate(angleVertical,vHorizontal.x(),0,-vHorizontal.y());

    Draw::translate(position);
}
