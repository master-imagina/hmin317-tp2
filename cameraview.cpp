#include "cameraview.h"

CameraView::CameraView(float x, float y, float z, float tx,float ty, float tz)
{
    CameraView::LookAt(x,y,z,tx,ty,tz);
}

void CameraView::LookAt(float x, float y, float z, float tx,float ty, float tz)
{
    Camera::LookAt(x,y,z,tx,ty,tz);

    distanceToTarget = (Camera::target - Camera::position).length();

    angleHorizontal = Camera::angleHorizontal;
    angleVertical = Camera::angleVertical;

    SetValues(distanceToTarget,angleHorizontal,angleVertical);
}

void CameraView::SetValues(float distance, float angleH, float angleV){

    distanceToTarget = distance;
    angleHorizontal = angleH;
    angleVertical = angleV;

    if(distanceToTarget < 0)
        distanceToTarget = 0;
    if(angleVertical < -89)
        angleVertical = -89;
    if(angleVertical > 89)
        angleVertical = 89;
    while(angleHorizontal < 0)
        angleHorizontal += 360;
    while(angleHorizontal > 360)
        angleHorizontal -= 360;

    float x = distanceToTarget * cos(angleHorizontal * M_PI / 180.0) * cos(angleVertical * M_PI / 180.0);
    float y = distanceToTarget * sin(angleVertical * M_PI / 180.0);
    float z = distanceToTarget * sin(angleHorizontal * M_PI / 180.0) * cos(angleVertical * M_PI / 180.0);


    Camera::LookAt(x,y,z,Camera::target.x(),Camera::target.y(),Camera::target.z());
}

void CameraView::Update(float addTodistance, float addToAngleH, float addToAngleV, float deplacement_speed, float angular_speed,float time_elapsed){

    distanceToTarget += addTodistance * deplacement_speed * time_elapsed / 1000.0;
    angleHorizontal += addToAngleH * angular_speed * time_elapsed / 1000.0;
    angleVertical += addToAngleV * angular_speed * time_elapsed / 1000.0;

    SetValues(distanceToTarget, angleHorizontal, angleVertical);
}
