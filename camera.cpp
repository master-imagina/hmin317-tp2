#include "camera.h"

#include <cmath>

Camera::Camera() :
    m_eyePos(),
    m_upVec(0.f, 1.f, 0.f),
    m_phi(0.f),
    m_theta(0.f),
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

QVector3D Camera::viewVector() const
{
    const float cosPhi = std::cos(m_phi);
    const float sinPhi = std::sin(m_phi);

    const float cosTheta = std::cos(m_theta);
    const float sinTheta = std::sin(m_theta);

    QVector3D ret(cosTheta * cosPhi,
                  sinTheta,
                  -cosTheta * sinPhi);
    ret.normalize();

    return ret;
}

QVector3D Camera::targetPos() const
{
    const QVector3D target = m_eyePos + viewVector();

    return target;
}

QVector3D Camera::rightVector() const
{
    const QVector3D ret = QVector3D::normal(viewVector(), m_upVec);

    return ret;
}

float Camera::phi() const
{
    return m_phi;
}

void Camera::setPhi(float phi)
{
    if (m_phi != phi) {
        m_phi = phi;

        m_isViewMatrixDirty = true;
    }
}

float Camera::theta() const
{
    return m_theta;
}

void Camera::setTheta(float theta)
{
    if (m_theta != theta) {
        m_theta = theta;

        m_isViewMatrixDirty = true;
    }
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
        m_viewMatrix.lookAt(m_eyePos, targetPos(), m_upVec);

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
