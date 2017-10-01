#include "coordconversions.h"

#include <cmath>


QVector3D sphericalToCartesian(float lon, float lat)
{
    const float cosLon = std::cos(lon);
    const float sinLon = std::sin(lon);

    const float cosLat = std::cos(lat);
    const float sinLat = std::sin(lat);

    QVector3D ret(sinLon * cosLat,
                  cosLon,
                  sinLon * sinLat);

    ret.normalize();

    return ret;
}

QVector2D cartesianToSpherical(const QVector3D &point)
{
    const float x = point.x();
    const float y = point.y();
    const float z = point.z();

    const float lon = std::atan2(y, x);
    const float lat = std::acos(z);

    const QVector2D ret(lon, lat);

    return ret;
}

QVector3D sphericalGLToCartesianGL(float lon, float lat)
{
    QVector3D ret = sphericalToCartesian(lon, lat);

    const float oldY = ret.y();
    const float oldZ = ret.z();

    ret.setY(oldZ);
    ret.setZ(oldY);

    return ret;
}
