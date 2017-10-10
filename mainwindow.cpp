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

#include "aabb.h"
#include "camera.h"
#include "cameracontroller.h"
#include "gameloop.h"
#include "gamewidget.h"
#include "geometry.h"
#include "heightmap.h"


MainWindow::MainWindow() :
    m_gameLoop(new GameLoop(1000, this)),
    m_terrainGeometry(std::make_unique<Geometry>()),
    m_gameWidgets(),
    m_camera(std::make_unique<Camera>()),
    m_cameraController(nullptr)
{
    m_camera->setEyePos({8, 20, 8});

    auto centralWidget = new QWidget(this);
    m_cameraController = new CameraController(centralWidget);
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
    QImage heightmap(filePath);

    if (heightmap.isNull() || !heightmap.isGrayscale()) {
        return;
    }

    *m_terrainGeometry = heightmapToGeometry(heightmap);

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->setRendererDirty();
    }
}

void MainWindow::pointCameraToTerrainCenter()
{
    AABoundingBox terrainAABB(m_terrainGeometry->vertices);

    const QVector3D terrainCenter = terrainAABB.center();
    const QVector3D flatCenter(terrainCenter.x(), 0.f, terrainCenter.z());

    const QVector3D newEye(flatCenter.x() + 50,
                           terrainAABB.yBounds().second + 50,
                           flatCenter.z() + 50);

    m_camera->setEyePos(newEye);
    m_camera->setTargetPos(flatCenter);
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
