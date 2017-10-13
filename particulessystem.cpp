#include "particulessystem.h"

ParticulesSystem::ParticulesSystem()
{

}

void ParticulesSystem::initParticuleSystem()
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();
    generateQuad();
}

void ParticulesSystem::cleanUp()
{
    delete heightGeneratorProgram;
}


QImage ParticulesSystem::proccessTextureParticles()
{



    QOpenGLFramebufferObject  * captureFBO;

    QOpenGLFramebufferObjectFormat format;
    format.setSamples(4);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    captureFBO = new QOpenGLFramebufferObject (PARTICLE_MAX, PARTICLE_MAX,format);


    particleProgram = new QOpenGLShaderProgram;
    // Compile vertex shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/simpleQuadVertex.glsl");


    // Compile fragment shader
    particleProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particleEngine.glsl");


    // Link shader pipeline
    particleProgram->link();


    // Bind shader pipeline for use
    particleProgram->bind();




    captureFBO->bind();


    glViewport(0, 0, PARTICLE_MAX, PARTICLE_MAX);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderQuad();
    captureFBO->release();



    particuleTexture = new QOpenGLTexture(captureFBO->toImage());
    return captureFBO->toImage();
}


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

void ParticulesSystem::generateQuad()
{

    int gridSize = 2;
    std::vector<VertexData> vertices;
    double step = 1.f / (gridSize-1);
    for(int i=0;i<gridSize;i++){
        for(int j=0;j<gridSize;j++){

            vertices.push_back({QVector3D((double)(i*step)*2.0 - 1.0,double(j*step)*2.0 - 1.0,0.0f),QVector2D(0,0)});
        }
    }

    std::vector<GLushort> indices;
    for(int i=0;i<gridSize-1;i++){
        for(int j=0;j<gridSize-1;j++){
            int topleft = i  + j*gridSize;
            int topright = topleft + 1;
            int bottomleft = i + (j+1)*gridSize;
            int bottomright = bottomleft+1;


            indices.push_back(topleft);
            indices.push_back(bottomright);
            indices.push_back(topright);

            indices.push_back(topleft);
            indices.push_back(bottomleft);
            indices.push_back(bottomright);
        }
    }



    m_nomberIndices = indices.size();
    arrayBuf.bind();
    arrayBuf.allocate(&vertices[0], vertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.size() * sizeof(GLushort));



}

void ParticulesSystem::renderQuad()
{
    srand (time(NULL));
    arrayBuf.bind();

    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = particleProgram->attributeLocation("a_position");
    particleProgram->enableAttributeArray(vertexLocation);
    particleProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    int texcoordLocation = particleProgram->attributeLocation("a_texcoord");
    particleProgram->enableAttributeArray(texcoordLocation);
    particleProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    particleProgram->setUniformValue("resolution", QVector2D(PARTICLE_MAX,PARTICLE_MAX));

    glDrawElements(GL_TRIANGLES, m_nomberIndices, GL_UNSIGNED_SHORT, 0);

    arrayBuf.destroy();
    indexBuf.destroy();
}
