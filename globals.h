#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <GL/glew.h>
#include "tilemap.h"
#include "tileset.h"
#include "stamp.h"

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 1000

#define SCREEN_WIDTH_TILES 8
#define SCREEN_HEIGHT_TILES 8

// GL objects
extern GLuint gShaderProgram;
extern GLuint gVAO;
extern GLuint gVBO;
extern GLuint gTilesetTex;

// Tileset info
extern int gTilesetWidth;
extern int gTilesetHeight;
extern int gSelectedTile;

// Stamp and state
extern std::vector<std::vector<uint16_t>> gTileStamp;

extern bool selectingStampArea;
extern bool placingStamp;
extern bool isSelecting;
extern int selectStartRow, selectStartCol, selectEndRow, selectEndCol;
extern int gTestPage;

extern Stamp stamp;

extern std::unique_ptr<Tilemap> tilemap;
extern std::unique_ptr<Tileset> tileset;
