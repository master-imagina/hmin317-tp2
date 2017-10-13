#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "global.h"
#include "camera.h"

class CameraView
{
    public:
        CameraView(float x, float y, float z, float tx,float ty, float tz);

        void LookAt(float x, float y, float z, float tx,float ty, float tz);

        float distanceToTarget;
        float angleHorizontal;
        float angleVertical;

        void SetValues(float,float,float);
        void Update(float=0,float=0,float=0,float=1,float=1,float=1);
};

#endif // CAMERAVIEW_H
