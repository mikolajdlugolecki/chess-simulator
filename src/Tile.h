#ifndef TILE_H
#define TILE_H

#define TILE_SIZE 0.5f
#define TILE_VERTEX_COUNT 6

float tileVertices[]={
    -TILE_SIZE,0.0f,TILE_SIZE,1.0f,
    -TILE_SIZE, 0.0f,-TILE_SIZE,1.0f,
    TILE_SIZE,0.0f,TILE_SIZE,1.0f,

    -TILE_SIZE,0.0f,-TILE_SIZE,1.0f,
    TILE_SIZE, 0.0f,-TILE_SIZE,1.0f,
    TILE_SIZE, 0.0f,TILE_SIZE,1.0f,
};

float tileColors[]={
    1.0f,1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,1.0f,

    1.0f,1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,1.0f,
};

float tileNormals[]={
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,

    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
};

float tileVertexNormals[]={
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,

    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
    0.0f, 1.0f,0.0f,0.0f,
};

float tileTexCoords[]={}; //TODO

#endif