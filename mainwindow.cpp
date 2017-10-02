#include "mainwindow.h"

#include <algorithm>

#include <QBoxLayout>
#include <QColor>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QtMath>

#include "camera.h"
#include "coordconversions.h"
#include "gamewidget.h"
#include "terraingeometry.h"


MainWindow::MainWindow() :
    m_terrainGeometry(std::make_unique<TerrainGeometry>()),
    m_gameWidgets()
{
    auto centralWidget = new QWidget(this);
    auto centralLayout = new QGridLayout(centralWidget);

    for (int i = 0; i < 4; ++i) {
        auto gameWidget = new GameWidget(std::pow(10, i), this);
        gameWidget->setObjectName("GameWidget" + QString::number(i));
        gameWidget->setGeometry(m_terrainGeometry.get());

        m_gameWidgets.push_back(gameWidget);

        centralLayout->addWidget(gameWidget, i / 2, i % 2);
    }

    setCentralWidget(centralWidget);

    createActions();
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

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->setRendererDirty();
    }
}

void MainWindow::pointCameraToTerrainCenter()
{
    const auto xBounds = m_terrainGeometry->widthBounds();
    const auto zBounds = m_terrainGeometry->depthBounds();
    const float maxTerrainHeight = m_terrainGeometry->heightBounds().second;

    const QVector3D center(xBounds.first + xBounds.second / 2,
                           0.f,
                           zBounds.first + zBounds.second / 2);
    const QVector3D newEye(center.x() + 50, maxTerrainHeight + 50, center.z() + 50);

    for (const GameWidget *gameWidget : m_gameWidgets) {
        Camera *camera = gameWidget->camera();
        camera->setEyePos(newEye);
        camera->setTargetPos(center);
    }
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
