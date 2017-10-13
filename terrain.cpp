#include "terrain.h"

Terrain::Terrain(){
}

Terrain::Terrain(string path, float size_x, float size_z, float height_max){
    Load(path,size_x,size_z,height_max);
}

void Terrain::Load(string path, float size_x, float size_z, float height_max){

    this->path = path;
    this->size_x = size_x;
    this->size_z = size_z;
    this->height_max = height_max;

    QImage hmap(path.c_str());

    float w =  hmap.width();
    float h =  hmap.height();

    std::vector<VertexData3D> v;

    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            float x = i * size_x/w - size_x/2;
            float y = (float)(hmap.pixel(i,j) & 0xff) * height_max / 255.0;
            float z = j * size_z/h - size_z/2;
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

    vbo_terrain.mode = GL_TRIANGLE_STRIP;
    vbo_terrain.indices = f;
    vbo_terrain.vertices = v;
}

void Terrain::Draw(){/*
    //GLuint array_faces[index.size()];
    GLuint* array_faces = (GLuint*)malloc(vbo_terrain.indices.size() * sizeof(GLuint));
    for(unsigned int i=0;i<vbo_terrain.indices.size();i++)
        array_faces[i] = vbo_terrain.indices[i];

    //VertexData3D array_vertices[vertices.size()];
    VertexData3D* array_vertices = (VertexData3D*)malloc(vbo_terrain.vertices.size() * sizeof(VertexData3D));
    for(unsigned int i=0;i<vbo_terrain.vertices.size();i++)
        array_vertices[i] = vbo_terrain.vertices[i];

    arrayBuf.bind();
    arrayBuf.allocate(array_vertices, vbo_terrain.vertices.size() * sizeof(VertexData3D));

    indexBuf.bind();
    indexBuf.allocate(array_faces, vbo_terrain.indices.size() * sizeof(GLuint));


    quintptr offset = 0;
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = Draw::program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData3D));

    offset += sizeof(QVector3D);
    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = Draw::program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData3D));

    // Draw cube geometry using indices from VBO 1
    openGLFunctions->glDrawElements(vbo_terrain.mode, vbo_terrain.indices.size(), GL_UNSIGNED_INT, 0);

    delete[] array_faces;
    delete[] array_vertices;*/
    Draw_3D::DrawVBO(vbo_terrain);
}
