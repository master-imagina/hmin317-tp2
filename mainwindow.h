#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

class GameWidget;
class TerrainGeometry;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private Q_SLOTS:
    void openLoadHeightMapDialog();
    void loadHeightMap(const QString &filePath);
    void pointCameraToTerrainCenter();

private:
    void createActions();

private:
    std::unique_ptr<TerrainGeometry> m_terrainGeometry;
    GameWidget *m_gameWidget;
};

#endif // MAINWINDOW_H
