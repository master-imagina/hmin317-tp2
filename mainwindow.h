#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

class Camera;
class CameraController;
class GameLoop;
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
    void iterateGameLoop();
    void createActions();

private:
    GameLoop *m_gameLoop;
    std::unique_ptr<TerrainGeometry> m_terrainGeometry;
    std::vector<GameWidget *> m_gameWidgets;
    std::unique_ptr<Camera> m_camera;

    CameraController *m_cameraController;
};

#endif // MAINWINDOW_H
