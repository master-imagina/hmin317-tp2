#include "mainwindow.h"

#include <algorithm>

#include <QColor>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "camera.h"
#include "coordconversions.h"
#include "gamewidget.h"
#include "terraingeometry.h"


MainWindow::MainWindow() :
    m_terrainGeometry(std::make_unique<TerrainGeometry>()),
    m_gameWidget(new GameWidget(this))
{
    m_gameWidget->setFocus();
    m_gameWidget->setGeometry(m_terrainGeometry.get());

    createActions();

    setCentralWidget(m_gameWidget);
}

MainWindow::~MainWindow()
{}

void MainWindow::openLoadHeightMapDialog()
{
    const QString filePath = QFileDialog::getOpenFileName(this,
                                                          QString(),
                                                          tr("Open HeightMap"),
                                                          tr("Image Files (*.png)"));

    if (filePath.isNull()) {
        return;
    }

    loadHeightMap(filePath);
}

void MainWindow::loadHeightMap(const QString &filePath)
{
    QImage heightMap(filePath);

    if (heightMap.isNull() || !heightMap.isGrayscale()) {
        return;
    }

    const int width = heightMap.width();
    const int height = heightMap.height();

    std::vector<int> heights(width * height);

    int counter = 0;
    std::generate(heights.begin(), heights.end(),
                  [&counter, &width, &heightMap] {
        const int x = counter / width;
        const int y = counter % width;

        counter++;

        return qGray(heightMap.pixel(x, y));
    });

    m_terrainGeometry->loadTerrainData(heights);
}

void MainWindow::pointCameraToTerrainCenter()
{
    const auto xBounds = m_terrainGeometry->widthBounds();
    const auto zBounds = m_terrainGeometry->depthBounds();
    const float maxTerrainHeight = m_terrainGeometry->heightBounds().second;

    const QVector3D center(xBounds.first + xBounds.second / 2 + 50,
                           0.f,
                           zBounds.first + zBounds.second / 2 + 50);

    Camera *camera = m_gameWidget->camera();
    const QVector3D newCameraEye(center.x(), maxTerrainHeight + 20, center.z());
    const QVector3D cameraDirection = center - newCameraEye;
    const QVector2D newCameraOrientation = cartesianToSpherical(cameraDirection);

    camera->setEyePos(newCameraEye);
    camera->setOrientation(newCameraOrientation);
}

void MainWindow::createActions()
{
    auto menuBar = new QMenuBar(this);

    // File menu
    auto fileMenu = new QMenu("&File", menuBar);
    QAction *openHeightMapAction = fileMenu->addAction("Load heightmap");
    openHeightMapAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(openHeightMapAction, &QAction::triggered,
            this, &MainWindow::openLoadHeightMapDialog);

    // Camera menu
    auto cameraMenu = new QMenu("&Camera", menuBar);
    QAction *targetTerrainCenterAction = cameraMenu->addAction("Target Terrain Center");
    targetTerrainCenterAction->setShortcut(QKeySequence("C"));
    connect(targetTerrainCenterAction, &QAction::triggered,
            this, &MainWindow::pointCameraToTerrainCenter);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(cameraMenu);

    setMenuBar(menuBar);
}
