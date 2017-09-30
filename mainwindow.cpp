#include "mainwindow.h"

#include <QColor>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "gamewidget.h"
#include "terraingeometry.h"


MainWindow::MainWindow() :
    m_terrainGeometry(std::make_unique<TerrainGeometry>()),
    m_gameWidget(new GameWidget(this))
{
    m_gameWidget->setFocus();
    m_gameWidget->setGeometry(m_terrainGeometry.get());

    auto menuBar = new QMenuBar(this);

    auto fileMenu = new QMenu("&File", menuBar);
    QAction *openHeightMapAction = fileMenu->addAction("Load heightmap");
    connect(openHeightMapAction, &QAction::triggered,
            this, &MainWindow::openLoadHeightMapDialog);

    menuBar->addMenu(fileMenu);

    setMenuBar(menuBar);
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
