#ifndef DRAWING_H
#define DRAWING_H

#include "graphicmanager.h"
#include "camera.h"
#include "global.h"

using namespace std;

class Draw {
    public:
        static void Initialization(QOpenGLShaderProgram* prg);
        static void Destroy();

        static void start3D();
        static void start2D();
        static void start2D(float x_min, float x_max, float y_min, float y_max, float z_near, float z_far);
        static void translate(float x, float y, float z);
        static void translate(QVector3D v);
        static void rotate(QQuaternion rotation);
        static void rotate(float angle,float x, float y, float z);
        static void loadIdentity();

        static void pushMatrix();
        static void popMatrix();

        static void setColor(QVector4D color);

        static QMatrix4x4 projection;
        static QMatrix4x4 matrix;

        static bool isInitialized;
        static bool is3DActive;
        static bool is2DActive;

        static void enableTexture(bool e);

        static QOpenGLBuffer arrayBuf;
        static QOpenGLBuffer indexBuf;

        static std::list<QMatrix4x4> matrixList;

        static QOpenGLFunctions* openGLFunctions;
        static QOpenGLShaderProgram *program;

        static void DrawVectors3D(GLenum mode, std::vector<VertexData3D> vertices, std::vector<GLuint> index);
        static void DrawVectors2D(GLenum mode, std::vector<VertexData2D> vertices, std::vector<GLuint> index);
};

namespace Draw_3D {

    void DrawVBO(VBO_3D);

    void Cube();

    void Plane(int,int);
    void Plane(string path);

    VBO_3D Init_Plane(int,int,float = 0.0);
    VBO_3D Init_Plane(string path);
}

namespace Draw_2D {

    void DrawVBO(VBO_2D);

    void Square(QVector2D Point1, QVector2D Point2);
}

#endif // DRAWING_H
