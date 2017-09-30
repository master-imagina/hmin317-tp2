#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameWidget;
class GeometryEngine;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() = default;

private Q_SLOTS:
    void openLoadHeightMapDialog();
    void loadHeightMap(const QString &filePath);

private:
    GeometryEngine *m_geometryEngine;
    GameWidget *m_gameWidget;
};

#endif // MAINWINDOW_H
