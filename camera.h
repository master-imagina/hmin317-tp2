#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    QVector3D eyePos() const;
    void setEyePos(const QVector3D &eyePos);

    QVector3D upVector() const;
    void setUpVector(const QVector3D &upVector);

    QVector3D viewVector() const;
    QVector3D targetPos() const;
    QVector3D rightVector() const;

    float phi() const;
    void setPhi(float phi);

    float theta() const;
    void setTheta(float theta);

    QVector2D orientation() const;
    void setOrientation(const QVector2D &orientation);

    float aspectRatio() const;
    void setAspectRatio(float aspectRatio);

    QMatrix4x4 viewMatrix();
    QMatrix4x4 projectionMatrix();

private:
    void syncUpVector();

private:
    QVector3D m_eyePos;
    QVector3D m_upVec;

    float m_phi;
    float m_theta;
    float m_aspectRatio;

    bool m_isViewMatrixDirty;
    bool m_isProjectionMatrixDirty;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
};

#endif // CAMERA_H
