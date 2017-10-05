#ifndef CAMERATHIRD_H
#define CAMERATHIRD_H
#include "utils.h"
#include "external/glm/mat4x4.hpp"
#include "camera.h"

class CameraThird: public Camera
{
public:
    CameraThird(float distanceFromPivot);
    CameraThird();
    void move(float dx, float dy , int dw, int z, int s, int q, int d, int dheigh);

    QMatrix4x4 getViewMatrix();

private:
    glm::vec3 lookAt;


    float distanceFromPivot;
    float angleAroundPivot;


    QMatrix4x4 viewMatrix;
    void calculateCameraPosition(float hD, float vD);
    float calculateHorizontal();
    float calculateVertical();
};

#endif // CAMERATHIRD_H
