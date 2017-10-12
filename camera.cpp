#include "camera.h"
#include <QtMath>
#include <iostream>

void Camera::processKeyPress(Camera_Movement movement) {
    float cameraSpeed = 0.3f;
    if(movement == Camera_Movement::Z) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if(movement == Camera_Movement::S) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if(movement == Camera_Movement::Q) {
        cameraPos -= QVector3D::crossProduct(cameraFront,cameraUp).normalized() * cameraSpeed;
    }
    if(movement == Camera_Movement::D) {
        cameraPos += QVector3D::crossProduct(cameraFront,cameraUp).normalized() * cameraSpeed;
    }
    if(movement == Camera_Movement::C) {
        cameraPos += cameraSpeed * cameraUp;
    }
    if(movement == Camera_Movement::W) {
        cameraPos -= cameraSpeed * cameraUp;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    float mouseSensitivity = 0.2f;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    }
    if(pitch < -89.0f) {
        pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::lookAt(QMatrix4x4 &matrix) {
    target = cameraPos + cameraFront;
    matrix.lookAt(cameraPos, target, cameraUp);
}

QVector3D Camera::getFront() {
    return cameraFront;
}

void Camera::orbitAround(QMatrix4x4 &matrix, float y, float p) {

    cameraPos += QVector3D::crossProduct(cameraFront,cameraUp).normalized() * 0.1f;
    processMouseMovement(-5.0f, 0.0f);
    updateCameraVectors();

    //a debug
/*
    matrix.rotate(p, cameraRight.normalized());
    cameraPos = (matrix * (cameraPos - target)) + target;
    matrix.rotate(y, cameraUp.normalized());
    cameraPos = (matrix * (cameraPos - target)) + target;
    matrix.lookAt(cameraPos, target, cameraUp);
*/
}

void Camera::updateCameraVectors() {
    QVector3D front;
    front.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    cameraFront = front.normalized();
    cameraRight = QVector3D::crossProduct(cameraFront, worldUp).normalized();
    cameraUp = QVector3D::crossProduct(cameraRight, cameraFront).normalized();
}
