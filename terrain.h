#ifndef TERRAIN_H
#define TERRAIN_H

#include "global.h"
#include "drawing.h"

class Terrain
{
public:
    Terrain();
    Terrain(string path,float size_x, float size_z, float height_max);

    void Load(string path,float size_x, float size_z, float height_max);
    void Draw();

    string path;
    float size_x;
    float size_z;
    float height_max;

    VBO_3D vbo_terrain;
};

#endif // TERRAIN_H
