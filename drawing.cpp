#include "drawing.h"

bool Draw::is3DActive = false;
bool Draw::is2DActive = false;
bool Draw::isInitialized = false;
QMatrix4x4 Draw::projection;
QMatrix4x4 Draw::matrix;
QOpenGLShaderProgram* Draw::program = NULL;
std::list<QMatrix4x4> Draw::matrixList;
QOpenGLFunctions* Draw::openGLFunctions = NULL;
QOpenGLBuffer Draw::arrayBuf;
QOpenGLBuffer Draw::indexBuf(QOpenGLBuffer::IndexBuffer);

void Draw::Initialization(QOpenGLShaderProgram* prg){
    arrayBuf.create();
    indexBuf.create();

    program = prg;
    openGLFunctions = new QOpenGLFunctions();
    openGLFunctions->initializeOpenGLFunctions();
}

void Draw::Destroy(){
    arrayBuf.destroy();
    indexBuf.destroy();
}

void Draw::start3D(){
    is3DActive = true;
    is2DActive = false;

    openGLFunctions->glEnable(GL_DEPTH_TEST);

    projection.setToIdentity();
    projection.perspective(45, GraphicManager::window_width/GraphicManager::window_height, 0.1, 1000);

    matrix.setToIdentity();

    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::start2D(){
    start2D(0,100,0,100,-1,1000);
}

void Draw::start2D(float x_min, float x_max, float y_min, float y_max, float z_near, float z_far){
    is2DActive = true;
    is3DActive = false;

    openGLFunctions->glDisable(GL_DEPTH_TEST);

    projection.setToIdentity();
    projection.ortho(x_min,x_max,y_min,y_max,z_near,z_far);

    matrix.setToIdentity();

    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::translate(QVector3D v){
    Draw::translate(v.x() ,v.y(),v.z());
}

void Draw::translate(float x, float y, float z){

    matrix.translate(x,y,z);
    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::rotate(QQuaternion rotation){

    matrix.rotate(rotation);
    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::rotate(float angle,float x, float y, float z){

    matrix.rotate(angle,x,y,z);
    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::loadIdentity(){

    matrix.setToIdentity();
    program->setUniformValue("mvp_matrix", projection*matrix);
}

void Draw::pushMatrix(){
    matrixList.push_back(matrix);
}

void Draw::popMatrix(){
    if(matrixList.size() > 0){
        matrix = matrixList.back();
        matrixList.pop_back();
    }
    else
        matrix.setToIdentity();
}

void Draw::setColor(QVector4D color){
    program->setUniformValue("color", color);
}

void Draw::enableTexture(bool e){
    program->setUniformValue("enableTexture", e);
}

void Draw::DrawVectors3D(GLenum mode, std::vector<VertexData3D> vertices, std::vector<GLuint> index){

    //GLuint array_faces[index.size()];
    GLuint* array_faces = (GLuint*)malloc(index.size() * sizeof(GLuint));
    for(unsigned int i=0;i<index.size();i++)
        array_faces[i] = index[i];

    //VertexData3D array_vertices[vertices.size()];
    VertexData3D* array_vertices = (VertexData3D*)malloc(vertices.size() * sizeof(VertexData3D));
    for(unsigned int i=0;i<vertices.size();i++)
        array_vertices[i] = vertices[i];

    Draw::arrayBuf.bind();
    Draw::arrayBuf.allocate(array_vertices, vertices.size() * sizeof(VertexData3D));

    Draw::indexBuf.bind();
    Draw::indexBuf.allocate(array_faces, index.size() * sizeof(GLuint));


    quintptr offset = 0;
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = Draw::program->attributeLocation("a_position");
    Draw::program->enableAttributeArray(vertexLocation);
    Draw::program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData3D));

    offset += sizeof(QVector3D);
    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = Draw::program->attributeLocation("a_texcoord");
    Draw::program->enableAttributeArray(texcoordLocation);
    Draw::program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData3D));

    // Draw cube geometry using indices from VBO 1
    Draw::openGLFunctions->glDrawElements(mode, index.size(), GL_UNSIGNED_INT, 0);

    delete[] array_faces;
    delete[] array_vertices;
}

void Draw::DrawVectors2D(GLenum mode, std::vector<VertexData2D> vertices, std::vector<GLuint> index){

    GLuint array_faces[index.size()];
    for(unsigned int i=0;i<index.size();i++)
        array_faces[i] = index[i];

    VertexData3D array_vertices[vertices.size()];
    for(unsigned int i=0;i<vertices.size();i++){
        array_vertices[i] = {QVector3D(vertices[i].position.x(),vertices[i].position.y(),-1) ,QVector2D(vertices[i].texCoord.x(),vertices[i].texCoord.y()) };
    }

    Draw::arrayBuf.bind();
    Draw::arrayBuf.allocate(array_vertices, vertices.size() * sizeof(VertexData3D));

    Draw::indexBuf.bind();
    Draw::indexBuf.allocate(array_faces, index.size() * sizeof(GLuint));


    quintptr offset = 0;
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = Draw::program->attributeLocation("a_position");
    Draw::program->enableAttributeArray(vertexLocation);
    Draw::program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData3D));

    offset += sizeof(QVector3D);
    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = Draw::program->attributeLocation("a_texcoord");
    Draw::program->enableAttributeArray(texcoordLocation);
    Draw::program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData3D));

    // Draw cube geometry using indices from VBO 1
    Draw::openGLFunctions->glDrawElements(mode, index.size(), GL_UNSIGNED_INT, 0);
}



void Draw_3D::DrawVBO(VBO_3D vbo){
    Draw::DrawVectors3D(vbo.mode,vbo.vertices,vbo.indices);
}

void Draw_3D::Cube(){

    std::vector<VertexData3D> v;
        // Vertex data for face 0
        v.push_back({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)});  // v0
        v.push_back({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}); // v1
        v.push_back({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)});  // v2
        v.push_back({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v3

        // Vertex data for face 1
        v.push_back({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}); // v4
        v.push_back({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}); // v5
        v.push_back({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)});  // v6
        v.push_back({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}); // v7

        // Vertex data for face 2
        v.push_back({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}); // v8
        v.push_back({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)});  // v9
        v.push_back({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}); // v10
        v.push_back({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)});  // v11

        // Vertex data for face 3
        v.push_back({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}); // v12
        v.push_back({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)});  // v13
        v.push_back({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}); // v14
        v.push_back({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)});  // v15

        // Vertex data for face 4
        v.push_back({QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}); // v16
        v.push_back({QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}); // v17
        v.push_back({QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v18
        v.push_back({QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}); // v19

        // Vertex data for face 5
        v.push_back({QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}); // v20
        v.push_back({QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}); // v21
        v.push_back({QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}); // v22
        v.push_back({QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)});  // v23


    std::vector<GLuint> f;
        f.push_back(0);f.push_back(1);f.push_back(2);f.push_back(3);f.push_back(3);
        f.push_back(4);f.push_back(4);f.push_back(5);f.push_back(6);f.push_back(7);f.push_back(7);
        f.push_back(8);f.push_back(8);f.push_back(9);f.push_back(10);f.push_back(11);f.push_back(11);
        f.push_back(12);f.push_back(12);f.push_back(13);f.push_back(14);f.push_back(15);f.push_back(15);
        f.push_back(16);f.push_back(16);f.push_back(17);f.push_back(18);f.push_back(19);f.push_back(19);
        f.push_back(20);f.push_back(20);f.push_back(21);f.push_back(22);f.push_back(23);


    Draw::DrawVectors3D(GL_TRIANGLE_STRIP,v,f);
}

void Draw_3D::Plane(int w, int h){

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i - w/2;
            float y = (float)(rand()%100) / 100.0;
            float z = j - h/2;
            float tx = (float)i / (float)(w-1);
            float ty = (float)j / (float)(h-1);

            v.push_back({QVector3D(x,y,z), QVector2D(tx, ty)});
        }
    }

    std::vector<GLuint> f;

    for(int i=0;i<w-1;i++){
        f.push_back( i*h );
        for(int j=0;j<h-1;j++){
            f.push_back( i*h+j );
            f.push_back( (i+1)*h+j );
            f.push_back( i*h+(j+1) );
            f.push_back( (i+1)*h+(j+1) );
        }
        f.push_back( (i+1)*h+(h-1) );
    }

    Draw::DrawVectors3D(GL_TRIANGLE_STRIP,v,f);
}

void Draw_3D::Plane(string path){

    QImage hmap(path.c_str());

    float w =  hmap.width();
    float h =  hmap.height();

        cout << "width  = " << w << endl;
        cout << "height = " << h << endl;

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i - w/2;
            float y = (float)(hmap.pixel(i,j) & 0xff) / 10.0;
            float z = j - h/2;
            float tx = (float)i / (float)(w-1);
            float ty = (float)j / (float)(h-1);

            v.push_back({QVector3D(x,y,z), QVector2D(tx, ty)});
        }
    }

    std::vector<GLuint> f;

    for(int i=0;i<w-1;i++){
        f.push_back( i*h );
        for(int j=0;j<h-1;j++){
            f.push_back( (i+1)*h+j );
            f.push_back( i*h+(j+1) );
        }
        f.push_back( (i+1)*h+(h-1) );
        f.push_back( (i+1)*h+(h-1) );
    }

    Draw::DrawVectors3D(GL_TRIANGLE_STRIP,v,f);
}

VBO_3D Draw_3D::Init_Plane(int w, int h, float mul_height){

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i - w/2;
            float y = mul_height * ((float)(rand()%100) / 100.0);
            float z = j - h/2;
            float tx = (float)i / (float)(w-1);
            float ty = (float)j / (float)(h-1);

            v.push_back({QVector3D(x,y,z), QVector2D(tx, ty)});
        }
    }

    std::vector<GLuint> f;

    for(int i=0;i<w-1;i++){
        f.push_back( i*h );
        for(int j=0;j<h-1;j++){
            f.push_back( i*h+j );
            f.push_back( (i+1)*h+j );
            f.push_back( i*h+(j+1) );
            f.push_back( (i+1)*h+(j+1) );
        }
        f.push_back( (i+1)*h+(h-1) );
    }

    VBO_3D vbo;
    vbo.mode = GL_TRIANGLE_STRIP;
    vbo.indices = f;
    vbo.vertices = v;

    return vbo;
}

VBO_3D Draw_3D::Init_Plane(string path){

    QImage hmap(path.c_str());

    float w =  hmap.width();
    float h =  hmap.height();

        cout << "width  = " << w << endl;
        cout << "height = " << h << endl;

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i - w/2;
            float y = (float)(hmap.pixel(i,j) & 0xff) / 10.0;
            float z = j - h/2;
            float tx = (float)i / (float)(w-1);
            float ty = (float)(h-1-j) / (float)(h-1);

            v.push_back({QVector3D(x,y,z), QVector2D(tx, ty)});
        }
    }

    std::vector<GLuint> f;

    for(int i=0;i<w-1;i++){
        f.push_back( i*h );
        for(int j=0;j<h-1;j++){
            f.push_back( i*h+j );
            f.push_back( (i+1)*h+j );
            f.push_back( i*h+(j+1) );
            f.push_back( (i+1)*h+(j+1) );
        }
        f.push_back( (i+1)*h+(h-1) );
    }

    VBO_3D vbo;
    vbo.mode = GL_TRIANGLE_STRIP;
    vbo.indices = f;
    vbo.vertices = v;

    return vbo;
}

void Draw_2D::DrawVBO(VBO_2D vbo){
    Draw::DrawVectors2D(vbo.mode,vbo.vertices,vbo.indices);
}

void Draw_2D::Square(QVector2D pt1, QVector2D pt2){

    std::vector<VertexData2D> v;
        v.push_back({QVector2D(pt1.x(), pt1.y()),QVector2D(0.0f, 0.0f)});
        v.push_back({QVector2D(pt1.x(), pt2.y()),QVector2D(0.0f, 1.0f)});
        v.push_back({QVector2D(pt2.x(), pt1.y()),QVector2D(1.0f, 0.0f)});
        v.push_back({QVector2D(pt2.x(), pt2.y()),QVector2D(1.0f, 1.0f)});

    std::vector<GLuint> f;
        f.push_back(0);
        f.push_back(1);
        f.push_back(2);
        f.push_back(3);

    Draw::DrawVectors2D(GL_TRIANGLE_STRIP,v,f);
}
