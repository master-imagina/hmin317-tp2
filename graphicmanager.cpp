#include "graphicmanager.h"

float GraphicManager::window_width = 1;
float GraphicManager::window_height = 1;

float absf(float nb){
    if(nb >= 0)
        return nb;
    return -nb;
}

GraphicManager::GraphicManager(QWidget *parent) :
    QOpenGLWidget(parent),
    texture(0)
{
    angle = 0;
    p_x = 0;
    p_y = 0;
}

GraphicManager::~GraphicManager(){
    makeCurrent();
    delete texture;
    delete terrain;
    delete cameraV;
    delete cameraFF;
    doneCurrent();
}

void GraphicManager::wheelEvent(QWheelEvent *event){

    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    InputManager::setWheel(numSteps,event->orientation() == Qt::Horizontal);
/*
    if (event->orientation() == Qt::Horizontal) {
        cout << "Horizontal = " << numSteps << endl;
    } else {
        cout << "Vertical = " << numSteps << endl;
    }
    event->accept();*/
}

void GraphicManager::mousePressEvent(QMouseEvent *e){

    InputManager::setButtonDown(e->button());

    mousePressPosition = QVector2D(e->localPos());
}

void GraphicManager::mouseMoveEvent(QMouseEvent *e){

    QVector2D diff = 0.5*(QVector2D(e->localPos()) - mousePressPosition);
    mousePressPosition = QVector2D(e->localPos());

    if(absf(diff.x()) > 0.05)
        p_x = diff.x();
    if(absf(diff.y()) > 0.05)
        p_y = diff.y();
}

void GraphicManager::mouseReleaseEvent(QMouseEvent *e){

    InputManager::setButtonUp(e->button());
/*
    QVector2D diff = 0.05 * (QVector2D(e->localPos()) - mousePressPosition);

    p_x = diff.x();
    p_y = diff.y();*/
}

void GraphicManager::timerEvent(QTimerEvent *) {

}

void GraphicManager::initializeGL(){

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Use QBasicTimer because its faster than QTimer
    timer.start();

    Draw::Initialization(&program);

    //vbo_plane = Draw_3D::Init_Plane(32,32,1.0);

    cameraV = new CameraView(0,0,15,0,0,0);
    //cameraFF = new CameraFreeFly(0,0,15,0,0,0);
    //cameraFF->useVerticalUp(false);

    terrain = new Terrain("heightmap-3.png",32,32,1);
    vbo_plane = Draw_3D::Init_Plane("heightmap-3.png");
}

void GraphicManager::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
}

void GraphicManager::initTextures()
{
    texture = new QOpenGLTexture(QImage(":/heightmap-3.png").mirrored());
        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::Repeat);
}

void GraphicManager::resizeGL(int w, int h)
{
    window_width = w;
    window_height = h;
}

void GraphicManager::keyPressEvent(QKeyEvent *e){
    InputManager::setKeyDown(e->key());
}

void GraphicManager::keyReleaseEvent(QKeyEvent *e){
    InputManager::setKeyUp(e->key());
}

int GraphicManager::getElapsedTime(){
    int timer_elapsed = timer.elapsed() - timer_prec;
    timer_prec = timer.elapsed();
    return timer_elapsed;
}

void GraphicManager::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();
    program.setUniformValue("texture", 0);
    Draw::setColor(QVector4D(1,1,1,1));

    Draw::start3D();
    Draw::enableTexture(true);

    int timer_elapsed = getElapsedTime();

        InputManager::Update(timer_elapsed);//*
        cameraV->Update(-InputManager::getVerticalWheel() + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_A,Qt::Key_E)*(-2),
                       p_x + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Q,Qt::Key_D),
                       p_y + InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Z,Qt::Key_S),
                       10.0,
                       20.0,
                       timer_elapsed);//*/
        /*
        cameraFF->Update(QVector3D(InputManager::getVariationBetweenKeys_Smooth(Qt::Key_Z,Qt::Key_S),
                                   InputManager::getVariationBetweenKeys_Smooth(Qt::Key_A,Qt::Key_E),
                                   InputManager::getVariationBetweenKeys_Smooth(Qt::Key_D,Qt::Key_Q)),
                         p_x,
                         -p_y,
                         1.0,
                         10.0,
                         timer_elapsed );//*/
        p_x *= 0.0;
        p_y *= 0.0;

        terrain->Draw();

    Draw::start2D(0,window_width,0,window_height,0.1,1000);

        Draw_2D::Square(QVector2D(0,0),QVector2D(200,200));
        Draw::enableTexture(false);
        Draw::setColor(QVector4D(1,1,0,1));
        Draw::translate(100-Camera::position.x()*200.0/terrain->size_x,100+Camera::position.z()*200.0/terrain->size_z,0);
        Draw_2D::Square(QVector2D(0,0),QVector2D(10,10));

    update();
}
