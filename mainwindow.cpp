#include "mainwindow.h"

#include <algorithm>

#include <QApplication>
#include <QBoxLayout>
#include <QColor>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QtMath>
#include <QTimer>

#include "camera.h"
#include "cameracontroller.h"
#include "coordconversions.h"
#include "gameloop.h"
#include "gamewidget.h"
#include "terraingeometry.h"


MainWindow::MainWindow() :
    m_gameLoop(new GameLoop(1000, this)),
    m_terrainGeometry(std::make_unique<TerrainGeometry>()),
    m_gameWidgets(),
    m_camera(std::make_unique<Camera>()),
    m_cameraController(nullptr)
{
    m_camera->setEyePos({8, 20, 8});

    m_cameraController = new CameraController(centralWidget);

    auto centralWidget = new QWidget(this);
    centralWidget->installEventFilter(m_cameraController);

    auto centralLayout = new QGridLayout(centralWidget);

    for (int i = 0; i < 4; i++) {
        auto gameWidget = new GameWidget(this);
        const int fps = std::pow(10, i);

        if (i < 3) {
            auto renderTimer = new QTimer(gameWidget);
            renderTimer->setInterval(1000 / fps);

            connect(renderTimer, &QTimer::timeout,
                    gameWidget, [gameWidget] {
                gameWidget->update();

                //FIXME Avoid file dialogs freezing. Implement threaded rendering instead
                qApp->processEvents();
            });

            renderTimer->start();
        }

        gameWidget->setObjectName("GameWidget" + QString::number(i));
        gameWidget->setGeometry(m_terrainGeometry.get());
        gameWidget->setCamera(m_camera.get());

        m_gameWidgets.push_back(gameWidget);

        centralLayout->addWidget(gameWidget, i / 2, i % 2);

        auto fpsLabel = new QLabel(QString::number(fps) + " fps", gameWidget);
        fpsLabel->setStyleSheet("QLabel { background-color : red }");
        fpsLabel->move(20, 20);
    }

    setCentralWidget(centralWidget);

    createActions();

    m_gameLoop->setCallback([this] { iterateGameLoop(); });
    m_gameLoop->run();

    centralWidget->setFocus();
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

    m_camera->setEyePos(newEye);
    m_camera->setTargetPos(center);
}

void MainWindow::iterateGameLoop()
{
    // Update scene
    m_cameraController->updateCamera(m_camera.get(), m_gameLoop->fps());

    // Render
    m_gameWidgets[3]->update();

    //FIXME Avoid file dialogs freezing. Implement threaded rendering instead
    qApp->processEvents();
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
