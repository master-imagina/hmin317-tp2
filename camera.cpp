#include "camera.h"

#include <cmath>


Camera::Camera() :
    m_eyePos(),
    m_targetPos(0.f, 0.f, 1.f),
    m_upVec(0.f, 1.f, 0.f),
    m_aspectRatio(1.f),
    m_isViewMatrixDirty(true),
    m_isProjectionMatrixDirty(true),
    m_viewMatrix(),
    m_projectionMatrix()
{}

QVector3D Camera::eyePos() const
{
    return m_eyePos;
}

void Camera::setEyePos(const QVector3D &eyePos)
{
    if (m_eyePos != eyePos) {
        m_eyePos = eyePos;

        m_isViewMatrixDirty = true;
    }
}

QVector3D Camera::upVector() const
{
    return m_upVec;
}

void Camera::setUpVector(const QVector3D &upVec)
{
    if (m_upVec != upVec) {
        m_upVec = upVec;

        m_isViewMatrixDirty = true;
    }
}

QVector3D Camera::targetPos() const
{
    return m_targetPos;
}

void Camera::setTargetPos(const QVector3D &targetPos)
{
    if (m_targetPos != targetPos) {
        m_targetPos = targetPos;

        m_isViewMatrixDirty = true;
    }
}

QVector3D Camera::viewVector() const
{
    return (m_targetPos - m_eyePos);
}

QVector3D Camera::rightVector() const
{
    const QVector3D ret = QVector3D::normal(viewVector().normalized(), m_upVec);

    return ret;
}

float Camera::aspectRatio() const
{
    return m_aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
    if (m_aspectRatio != aspectRatio) {
        m_aspectRatio = aspectRatio;

        m_isProjectionMatrixDirty = true;
    }
}

QMatrix4x4 Camera::viewMatrix()
{
    if (m_isViewMatrixDirty) {
        m_viewMatrix.setToIdentity();
        m_viewMatrix.lookAt(m_eyePos, m_targetPos, m_upVec);

        m_isViewMatrixDirty = false;
    }

    return m_viewMatrix;
}

QMatrix4x4 Camera::projectionMatrix()
{
    if (m_isProjectionMatrixDirty) {
        m_projectionMatrix.setToIdentity();
        m_projectionMatrix.perspective(45.f, m_aspectRatio, 1.f, 10000.f);

        m_isProjectionMatrixDirty = false;
    }

    return m_projectionMatrix;
}
