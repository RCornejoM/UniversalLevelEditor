// tileset.h
#pragma once
#include <string>
#include <iostream>
#include "config.h"
#include "stb_image.h"
#include <GL/glew.h>

class Tileset {
public:
    bool load(std::string& filename, int tileSize);
    bool hasLoaded() const { return loaded; }

    GLuint getTextureID() const { return textureID; }
    int getWidthTiles() const { return widthTiles; }
    int getHeightTiles() const { return heightTiles; }
    int getTotalTiles() const { return totalTiles; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTileSize() const { return tileSize; }
    std::string getFilename() const { return filename;};
    GLuint textureID;
    int width;
    int height;
private:
    std::string filename;
    int tileSize;


    int widthTiles;
    int heightTiles;
    int totalTiles;
    bool loaded = false;
};
