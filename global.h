#ifndef GLOBAL_H
#define GLOBAL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QVector2D>
#include <QVector3D>
#include <vector>
#include <QOpenGLBuffer>
#include <list>
#include <iostream>
#include <QImage>
#include <string>
#include <QElapsedTimer>

using namespace std;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

struct VertexData3D
{
    QVector3D position;
    QVector2D texCoord;
    //QVector3D normal;
};

struct VertexData2D
{
    QVector2D position;
    QVector2D texCoord;
};

struct VBO_3D
{
    GLenum mode;
    vector<VertexData3D> vertices;
    vector<GLuint> indices;
};

struct VBO_2D
{
    GLenum mode;
    vector<VertexData2D> vertices;
    vector<GLuint> indices;
};

class Global
{
public:
    Global();
};

#endif // GLOBAL_H
