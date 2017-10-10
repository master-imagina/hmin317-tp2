Moteurs de jeux : compte-rendu de TP2
=====================================


![The main window](mainwindow.png)


Question 1
----------

`File | Load heightmap (Ctrl+I)` permet à l'utilisateur d'importer une 
heightmap.

Si l'image est en niveaux de gris, après création de la géométrie du terrain,
le Renderer de chaque QOpenGLWidget met à jour les buffers destinés au GPU.

```cpp
void MainWindow::loadHeightMap(const QString &filePath)
{
    QImage heightMap(filePath);

    if (heightMap.isNull() || !heightMap.isGrayscale()) {
        return;
    }

    const int width = heightMap.width();
    const int height = heightMap.height();

    std::vector<int> heights(width * height);

    int pxCounter = 0;
    std::generate(heights.begin(), heights.end(),
                  [&pxCounter, &width, &heightMap] {
        const int x = pxCounter / width;
        const int y = pxCounter % width;

        pxCounter++;

        return qGray(heightMap.pixel(x, y));
    });

    m_terrainGeometry->loadTerrainData(heights);

    for (GameWidget *gameWidget : m_gameWidgets) {
        gameWidget->setRendererDirty();
    }
}
```

Question 2
----------

`Camera | Target Terrain Center (C)` pour positionner la caméra au dessus du
terrain et cibler son centre.

```cpp
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
```

(les méthodes d'analyse de géométrie seront découplées de la classe
TerrainGeometry)


La touche R du clavier permet de faire tourner la caméra autour de sa cible.

```cpp
void rotateCameraAroundTarget(Camera *camera, float angle)
{
    const QVector3D axis = camera->upVector();
    const QQuaternion quat = QQuaternion::fromAxisAndAngle(axis, angle);

    camera->setUpVector(quat * axis);
    camera->setEyePos(camera->targetPos() - quat * camera->viewVector());
}
```

Cette méthode est appelée lors de la mise à jour de la caméra en fonction des
actions de l'utilisateur :

```cpp
void CameraController::updateCamera(Camera *camera, unsigned int fps)
{
    [...] // Code for updating the camera if the user uses ZQSD

    if (m_isRotateAroundTargetPressed) {
        const float angle = 2.f * factor * m_rotateSpeed;

        rotateCameraAroundTarget(camera, angle);
    }
}
```


Question 3
----------

La mise à jour du rendu est contrôlé par l'attribut m_timer, de type QTimer.
Le timer est lié au MainWidget pour que la méthode timerEvent() soit appelée à
chaque fois que le timer est fini.

Cela dit, j'ai préféré découpler la logique de game loop de la logique 
d'affichage OpenGL (le QOpenGLWidget). Pour cela, j'utilise un timer principal 
qui fera office de game loop, d'une fréquence de 1000 FPS, soit la plus élevée 
des quatre viewports. Cette game loop procède à la mise à jour de la scène puis 
au rendu du viewport le plus fluide.

Les trois autres viewports, de fréquence 1, 10 et 100 FPS ont leur propre timer.

Chaque QOpenGLWidget dispose de son propre Renderer et de son propre contexte
OpenGL. Afin d'optimiser le tout, il est possible de partager les ressources 
entre les quatre contextes OpenGL, mais cela nécessite un gros travail au 
niveau de l'architecture du moteur de rendu, car certains objets OpenGL ne sont 
pas partageables entre les contextes, comme les VAO et les shader programs.


Un viewport avec une faible fréquence d'affichage peut mettre plus de temps
pour présenter le résultat du rendu à l'utilisateur.


Bonus
-----

Le fragment shader `res/shaders/terrain_heightmap.frag` applique un effet 
"heightmap" à une géométrie. Plus la hauteur d'un sommet est élevée, plus le
pixel associé sera blanc.

```glsl
uniform float minHeight;            // Terrain minimum height
uniform float maxHeight;            // Terrain maximum height

varying float vertexHeight;

void main()
{
    float a = 255. / maxHeight;
    float b = (255. * minHeight) / (maxHeight - minHeight);

    float grayscaleValue = floor(a * vertexHeight + b) / 255.;

    gl_FragColor = vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0);
}
```
