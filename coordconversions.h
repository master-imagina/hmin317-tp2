#ifndef COORDCONVERSIONS_H
#define COORDCONVERSIONS_H

#include <QVector2D>
#include <QVector3D>


QVector3D sphericalToCartesian(float lon, float lat);
QVector3D sphericalGLToCartesianGL(float lon, float lat);

QVector2D cartesianToSpherical(const QVector3D &point);

#endif // COORDCONVERSIONS_H
