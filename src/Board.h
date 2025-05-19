#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

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

#endif
