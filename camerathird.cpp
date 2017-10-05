#include "camerathird.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

CameraThird::CameraThird(float distanceFromPivot){
    this->distanceFromPivot = distanceFromPivot,
    position.x=0;position.y=1;position.z=5;
    lookAt.x=0;lookAt.y=0;lookAt.z=0;
    pitch=45;
    angleAroundPivot=180;

}

CameraThird::CameraThird()
{
    position.x=0;position.y=1;position.z=5;
    lookAt.x=128;lookAt.y=0;lookAt.z=128;
    pitch=45;
    angleAroundPivot=40;
    distanceFromPivot=40;

}


void CameraThird::move(float dx, float dy, int dw, int z, int s, int q, int d, int dheigh){
    float zoomLevel = dw;

    if(distanceFromPivot - zoomLevel > 1.0f && distanceFromPivot - zoomLevel < 3000 )
        distanceFromPivot -= zoomLevel;



    float pitchChange = dy * 0.3f;
    if(pitch - pitchChange >0 && pitch - pitchChange <90)
        pitch -= pitchChange;
    float angleChange = dx * 0.3f;
    angleAroundPivot -= angleChange;


    if(z){

        lookAt.z += 1.5f * cos(Utils::toRadians(angleAroundPivot));

        lookAt.x += 1.5f * sin(Utils::toRadians(angleAroundPivot));

    }
    if(s){

        lookAt.z -= 1.5f * cos(Utils::toRadians(angleAroundPivot));

        lookAt.x -= 1.5f * sin(Utils::toRadians(angleAroundPivot));
    }
    if(q){

        lookAt.z -= 1.5f * sin(Utils::toRadians(angleAroundPivot));

        lookAt.x += 1.5f * cos(Utils::toRadians(angleAroundPivot));
    }
    if(d){

        lookAt.z += 1.5f * sin(Utils::toRadians(angleAroundPivot));

        lookAt.x -= 1.5f * cos(Utils::toRadians(angleAroundPivot));

    }

    float horizontalDistance = calculateHorizontal();
    float verticalDistance = calculateVertical();
    calculateCameraPosition(horizontalDistance,verticalDistance);

    yaw = 180 - angleAroundPivot;
}

/*
 * Simply Use Sinus law to calculate the camera position
 * Shift the lookAt position
 */
void CameraThird::calculateCameraPosition(float hD, float vD){
    float offsetX = (float) (hD * sin(Utils::toRadians(angleAroundPivot)));
    float offsetZ = (float) (hD * cos(Utils::toRadians(angleAroundPivot)));
    position.x = lookAt.x - offsetX;
    position.y = lookAt.y + vD;
    position.z = lookAt.z - offsetZ;
}

float CameraThird::calculateHorizontal(){
    return (float) (distanceFromPivot * cos(Utils::toRadians(pitch)));
}

float CameraThird::calculateVertical(){
    return (float) (distanceFromPivot * sin(Utils::toRadians(pitch)));
}

QMatrix4x4 CameraThird::getViewMatrix(){
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(position.x,position.y,position.z),QVector3D(lookAt.x,lookAt.y,lookAt.z),QVector3D(0,1,0));

    return viewMatrix;
}

