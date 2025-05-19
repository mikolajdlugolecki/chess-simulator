#ifndef TILE_H
#define TILE_H

#define TILE_SIZE 0.5f
#define TILE_VERTEX_COUNT 6

float tileVertices[] = {
    -TILE_SIZE, 0.f, TILE_SIZE, 1.f,
    -TILE_SIZE, 0.f, -TILE_SIZE, 1.f,
    TILE_SIZE, 0.f, TILE_SIZE, 1.f,

    -TILE_SIZE, 0.f, -TILE_SIZE, 1.f,
    TILE_SIZE, 0.f, -TILE_SIZE, 1.f,
    TILE_SIZE, 0.f, TILE_SIZE, 1.f,
};

float tileColors[] = {
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,

    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
};

float tileNormals[] = {
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
};

float tileVertexNormals[] = {
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
};

float tileTexCoords[] = {
    0.f, 1.f,
    0.f, 0.f,
    1.f, 1.f,

    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f,
};

#endif