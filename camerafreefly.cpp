#include "camerafreefly.h"

CameraFreeFly::CameraFreeFly(float x, float y, float z, float tx,float ty, float tz){
    CameraFreeFly::LookAt(x,y,z,tx,ty,tz);
    _useVerticalUp = true;
}

void CameraFreeFly::LookAt(float x, float y, float z, float tx,float ty, float tz) {

    Camera::LookAt(x,y,z,tx,ty,tz);

    position = QVector3D(x,y,z);
    target = QVector3D(tx,ty,tz);

    angleHorizontal = Camera::angleHorizontal;
    angleVertical = Camera::angleVertical;

    SetValues(position,angleHorizontal,angleVertical);
}

void CameraFreeFly::SetValues(QVector3D position, float angleH, float angleV){

    this->position = position;
    angleHorizontal = angleH;
    angleVertical = angleV;

    if(angleVertical < -89)
        angleVertical = -89;
    if(angleVertical > 89)
        angleVertical = 89;
    while(angleHorizontal < 0)
        angleHorizontal += 360;
    while(angleHorizontal > 360)
        angleHorizontal -= 360;

    float tx = position.x() + cos(angleHorizontal * M_PI / 180.0) * cos(angleVertical * M_PI / 180.0);
    float ty = position.y() + sin(angleVertical * M_PI / 180.0);
    float tz = position.z() + sin(angleHorizontal * M_PI / 180.0) * cos(angleVertical * M_PI / 180.0);

    target = QVector3D(tx,ty,tz);

    Camera::LookAt(position.x(),position.y(),position.z(),target.x(),target.y(),target.z());
}

void CameraFreeFly::Update(QVector3D addToposition, float addToAngleH, float addToAngleV, float deplacement_speed, float angular_speed,float time_elapsed){

    QVector3D direction = (target - position).normalized();
    QVector3D normal    = QVector3D(0,1,0);;
    QVector3D lateral   = QVector3D::crossProduct(direction,normal).normalized();;

    if(_useVerticalUp){ // FPS
        direction   = QVector3D(direction.x(),0,direction.z()).normalized();
    }

    position += direction * addToposition.x() * deplacement_speed * time_elapsed / 1000.0;
    position += lateral * addToposition.z() * deplacement_speed * time_elapsed / 1000.0;
    position += normal * addToposition.y() * deplacement_speed * time_elapsed / 1000.0;

    angleHorizontal += addToAngleH * angular_speed * time_elapsed / 1000.0;
    angleVertical += addToAngleV * angular_speed * time_elapsed / 1000.0;

    SetValues(position, angleHorizontal, angleVertical);
}

void CameraFreeFly::useVerticalUp(bool b){
    _useVerticalUp = b;
}

