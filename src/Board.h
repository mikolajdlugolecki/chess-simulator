#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"
#include <map>

#define BLACK_TILE_COLOR_R 0.3841f
#define BLACK_TILE_COLOR_G 0.1647f
#define BLACK_TILE_COLOR_B 0.0588f

#define WHITE_TILE_COLOR_R 0.6f
#define WHITE_TILE_COLOR_G 0.4745f
#define WHITE_TILE_COLOR_B 0.3137f

#define BOARD_SIZE 8
#define BOARD_VERTEX_COUNT BOARD_SIZE * BOARD_SIZE * TILE_VERTEX_COUNT

float boardVertices[BOARD_VERTEX_COUNT * 4];
float boardNormals[BOARD_VERTEX_COUNT * 4];
float boardColors[BOARD_VERTEX_COUNT * 4];
float boardTexCoords[BOARD_VERTEX_COUNT * 2];
float boardTextures[BOARD_VERTEX_COUNT];

std::map<char, int> boardMap = {
	{'a', 0},
	{'b', 1},
	{'c', 2},
	{'d', 3},
	{'e', 4},
	{'f', 5},
	{'g', 6},
	{'h', 7}
};

int boardBaseVertexCount = 48;

float boardBaseVertices[] = {
    -1.f, 0.125f, 1.f, 1.f,
    -1.f, 0.125f, -1.f, 1.f,
    0.f, 0.125f, 0.f, 1.f,

    -1.f, 0.125f, -1.f, 1.f,
    1.f, 0.125f, -1.f, 1.f,
    0.f, 0.125f, 0.f, 1.f,

    1.f, 0.125f, 1.f, 1.f,
    1.f, 0.125f, -1.f, 1.f,
    0.f, 0.125f, 0.f, 1.f,

    -1.f, 0.125f, 1.f, 1.f,
    1.f, 0.125f, 1.f, 1.f,
    0.f, 0.125f, 0.f, 1.f,

    -1.f, -0.125f, 1.f, 1.f,
    -1.f, -0.125f, -1.f, 1.f,
    0.f, -0.125f, 0.f, 1.f,

    -1.f, -0.125f, -1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,
    0.f, -0.125f, 0.f, 1.f,

    1.f, -0.125f, 1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,
    0.f, -0.125f, 0.f, 1.f,

    -1.f, -0.125f, 1.f, 1.f,
    1.f, -0.125f, 1.f, 1.f,
    0.f, -0.125f, 0.f, 1.f,

    -1.f, 0.125f, 1.f, 1.f,
    -1.f, -0.125f, 1.f, 1.f,
    -1.f, -0.125f, -1.f, 1.f,

    -1.f, 0.125f, 1.f, 1.f,
    -1.f, 0.125f, -1.f, 1.f,
    -1.f, -0.125f, -1.f, 1.f,

    1.f, 0.125f, 1.f, 1.f,
    1.f, -0.125f, 1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,

    1.f, 0.125f, 1.f, 1.f,
    1.f, 0.125f, -1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,

    -1.f, 0.125f, -1.f, 1.f,
    -1.f, -0.125f, -1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,

    -1.f, 0.125f, -1.f, 1.f,
    1.f, 0.125f, -1.f, 1.f,
    1.f, -0.125f, -1.f, 1.f,

    -1.f, 0.125f, 1.f, 1.f,
    -1.f, -0.125f, 1.f, 1.f,
    1.f, -0.125f, 1.f, 1.f,

    -1.f, 0.125f, 1.f, 1.f,
    1.f, 0.125f, 1.f, 1.f,
    1.f, -0.125f, 1.f, 1.f,
};

float boardBaseNormals[] = {
    0.f, 0.f, -1.f, 0.f,
    0.f, 0.f, -1.f, 0.f,
    0.f, 0.f, -1.f, 0.f,

    0.f, 0.f, -1.f, 0.f,
    0.f, 0.f, -1.f, 0.f,
    0.f, 0.f, -1.f, 0.f,

    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 1.f, 0.f,

    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 1.f, 0.f,

    0.f, -1.f, 0.f, 0.f,
    0.f, -1.f, 0.f, 0.f,
    0.f, -1.f, 0.f, 0.f,

    0.f, -1.f, 0.f, 0.f,
    0.f, -1.f, 0.f, 0.f,
    0.f, -1.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,

    -1.f, 0.f, 0.f, 0.f,
    -1.f, 0.f, 0.f, 0.f,
    -1.f, 0.f, 0.f, 0.f,

    -1.f, 0.f, 0.f, 0.f,
    -1.f, 0.f, 0.f, 0.f,
    -1.f, 0.f, 0.f, 0.f,

    1.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 0.f, 0.f,

    1.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 0.f, 0.f,

};

float boardBaseTexCoords[] = {
    0.f, 0.f,
    1.f, 0.f,
    0.5f, 1.f,

    0.f, 0.f,
    1.f, 0.f,
    0.5f, 1.f,

    1.f, 0.f,
    0.f, 0.f,
    0.5f, 1.f,

    1.f, 0.f,
    0.f, 0.f,
    0.5f, 1.f,

    0.f, 0.f,
    1.f, 0.f,
    0.5f, 1.f,

    0.f, 0.f,
    1.f, 0.f,
    0.5f, 1.f,

    1.f, 0.f,
    0.f, 0.f,
    0.5f, 1.f,

    1.f, 0.f,
    0.f, 0.f,
    0.5f, 1.f,

    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,

    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,

    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,

    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,

    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,

    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,

    0.f, 1.f,
    0.f, 0.f,
    1.f, 0.f,

    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,
};

#endif
