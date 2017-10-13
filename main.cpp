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

    app.setApplicationName("TP2");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWidget widget1(nullptr, 1);
    MainWidget widget10(nullptr, 10);
    MainWidget widget30(nullptr, 30);
    MainWidget widget100(nullptr, 100);

    widget1.show();
    widget10.show();
    widget30.show();
    widget100.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
