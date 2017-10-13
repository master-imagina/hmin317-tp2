#ifndef CAMERAFREEFLY_H
#define CAMERAFREEFLY_H

#include "global.h"
#include "camera.h"

class CameraFreeFly
{
    public:
        CameraFreeFly(float x, float y, float z, float tx,float ty, float tz);

        void LookAt(float x, float y, float z, float tx,float ty, float tz);

        QVector3D position;
        QVector3D target;
        float angleHorizontal;
        float angleVertical;

        void SetValues(QVector3D,float,float);
        void Update(QVector3D=QVector3D(0,0,0),float=0,float=0,float=1,float=1,float=1);

        void useVerticalUp(bool);

    private:
        bool _useVerticalUp;
};

#endif // CAMERAFREEFLY_H
