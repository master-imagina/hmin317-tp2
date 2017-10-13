#include <QApplication>

#include "graphicmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RPSoundManager");

    GraphicManager gManager;
    gManager.show();

    return app.exec();
}
