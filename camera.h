#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <iostream>

#include "drawing.h"

using namespace std;

class Camera
{
    public:
        Camera();

        static void LookAt(float x, float y, float z, float tx,float ty, float tz);

        static QVector3D position;
        static QVector3D target;

        static float angleHorizontal;
        static float angleVertical;
};

#endif // CAMERA_H
