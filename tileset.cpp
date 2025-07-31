#include "tileset.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

bool Tileset::load(std::string& filename, int tileSize) {
    std::cout << "[Tileset] Loading image: " << filename << "\n";

    this->filename = filename;
    this->tileSize = tileSize;

    if (textureID != 0) {
        std::cout << "[Tileset] Deleting old texture ID: " << textureID << "\n";
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    int widthPx = 0, heightPx = 0, channels = 0;
    unsigned char* data = stbi_load(filename.c_str(), &widthPx, &heightPx, &channels, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "[Tileset] Failed to load image data: " << filename << "\n";
        return false;
    }

    std::cout << "[Tileset] Image loaded successfully\n";
    std::cout << "  Width: " << widthPx << "\n";
    std::cout << "  Height: " << heightPx << "\n";
    std::cout << "  Original Channels: " << channels << "\n";
    std::cout << "  Using 4 channels (RGBA) for OpenGL\n";

    this->width = widthPx;
    this->height = heightPx;

    glGenTextures(1, &textureID);
    std::cout << "[Tileset] Generated new texture ID: " << textureID << "\n";

    glBindTexture(GL_TEXTURE_2D, textureID);
    std::cout << "[Tileset] Bound texture ID " << textureID << " to GL_TEXTURE_2D\n";

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPx, heightPx, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    std::cout << "[Tileset] Uploaded texture image to GPU\n";

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    std::cout << "[Tileset] Set texture parameters: MIN_FILTER=NEAREST, MAG_FILTER=NEAREST, WRAP=CLAMP_TO_EDGE\n";

    stbi_image_free(data);
    std::cout << "[Tileset] Freed image data from CPU memory\n";

    widthTiles = widthPx / tileSize;
    heightTiles = heightPx / tileSize;
    totalTiles = widthTiles * heightTiles;

    std::cout << "[Tileset] Calculated tile counts:\n"
              << "  Tile Size: " << tileSize << "\n"
              << "  Width in tiles: " << widthTiles << "\n"
              << "  Height in tiles: " << heightTiles << "\n"
              << "  Total tiles: " << totalTiles << "\n";

    std::cout << "[Tileset] Finished loading tileset: " << filename << "\n";

    return true;
}
