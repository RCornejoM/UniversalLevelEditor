#include "globals.h"

// GL objects
GLuint gShaderProgram = 0;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint gTilesetTex = 0;

// Tileset info
int gTilesetWidth = 0;
int gTilesetHeight = 0;
int gSelectedTile = 0;

// Stamp and state
std::vector<std::vector<uint16_t>> gTileStamp;

bool selectingStampArea = false;
bool placingStamp = false;
bool isSelecting = false;
int selectStartRow = 0, selectStartCol = 0, selectEndRow = 0, selectEndCol = 0;
int gTestPage = 0;

Stamp stamp;
std::unique_ptr<Tilemap> tilemap = std::make_unique<Tilemap>();
std::unique_ptr<Tileset> tileset = std::make_unique<Tileset>();
