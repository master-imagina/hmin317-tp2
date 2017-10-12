#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("plane");
    app.setApplicationVersion("0.1");


#ifndef QT_NO_OPENGL
    MainWidget widget1;
    widget1.setWindowTitle("Widget 1");
    widget1.setFPS(60);
    widget1.show();



    MainWidget widget2;
    widget2.setWindowTitle("Widget 2");
    widget2.setFPS(40);
    widget2.show();


    MainWidget widget3;
    widget3.setWindowTitle("Widget 3");
    widget3.setFPS(20);
    widget3.show();


    MainWidget widget4;
    widget4.setWindowTitle("Widget 4");
    widget4.setFPS(10);
    widget4.show();


#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
