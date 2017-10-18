#ifndef CAMERA_H
#define CAMERA_H
#include "external/glm/mat4x4.hpp"
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    glm::vec3 getPosition();
    float getPitch();
    float getYaw();
    float getRoll();
    void invertRoll();
    float getFOV();
    float getNEAR_PLANE();
    float getFAR_PLANE();
    void setProjectionMatrix(QMatrix4x4 projectionMatrix);
    QMatrix4x4 getProjectionMatrix();

protected:
    glm::vec3 position;
    glm::vec3 lookAt;
    float pitch;
    float roll;
    float yaw;
    float FOV;
    float NEAR_PLANE;
    float FAR_PLANE;

private:




    QMatrix4x4 projectionMatrix;
};

#endif // CAMERA_H
