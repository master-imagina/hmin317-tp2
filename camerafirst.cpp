#include "camerafirst.h"

#include "camerathird.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

CameraFirst::CameraFirst(float distanceFromPivot){
    this->distanceFromPivot = distanceFromPivot,
    position.x=10;position.y=10;position.z=10;
    lookAt.x=0;lookAt.y=0;lookAt.z=0;
    pitch=40;
    angleAroundPivot=180;

}

CameraFirst::CameraFirst()
{
    position.x=10;position.y=10;position.z=10;
    lookAt.x=0;lookAt.y=0;lookAt.z=0;
    pitch=20;
    angleAroundPivot=40;
    distanceFromPivot=20;

}


void CameraFirst::move(float dx, float dy, int dw, int z, int s, int q, int d,int dHeigh){

    pitch += dy * 0.3f;
    yaw += dx * 0.3f;


    if(pitch > 89.0)
         pitch = 89.0;
    else if(pitch < -89.0)
        pitch = -89.0;
    float phiRadian = Utils::toRadians(pitch);
    float thetaRadian = Utils::toRadians(yaw);

    lookAt.x = cos(phiRadian) * -sin(thetaRadian);

    lookAt.y = -sin(phiRadian);

    lookAt.z = cos(phiRadian) * cos(thetaRadian);

    glm::vec3 right;
    right.x = sin(thetaRadian- 3.14f/2.0f);
    right.y = 0;
    right.z = -cos(thetaRadian- 3.14f/2.0f);

    if(z){

        position.x += lookAt.x *1.5f;
        position.y += lookAt.y *1.5f ;
        position.z += lookAt.z *1.5f ;

    }
    if(s){

        position.x -= lookAt.x *1.5f;
        position.y -= lookAt.y *1.5f ;
        position.z -= lookAt.z *1.5f ;

    }
    if(q){
        position.x -= right.x *1.5f;

        position.z -= right.z *1.5f ;
    }
    if(d){

        position.x += right.x *1.5f;

        position.z += right.z *1.5f ;

    }


    position.y += 0.7f*dHeigh;



}



QMatrix4x4 CameraFirst::getViewMatrix(){
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(position.x,position.y,position.z),QVector3D(position.x+lookAt.x,position.y+lookAt.y,position.z+lookAt.z),QVector3D(0,1,0));

    return viewMatrix;
}

