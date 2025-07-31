#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

#include "config.h"

constexpr uint16_t HFLIP_MASK = 0x4000;
constexpr uint16_t VFLIP_MASK = 0x8000;

struct Tile {
    uint16_t raw;

    int index() const  { return raw & 0x03FF; }
    bool hflip() const { return raw & 0x4000; }
    bool vflip() const { return raw & 0x8000; }

    Tile(int idx = 0, bool h = false, bool v = false) {
        raw = static_cast<uint16_t>(
            (idx & 0x03FF) |
            (h ? 0x4000 : 0) |
            (v ? 0x8000 : 0)
        );
    }

    void flipHorizontally() {
        raw ^= HFLIP_MASK;
    }

    void flipVertically() {
        raw ^= VFLIP_MASK;
    }
};

class Tilemap {
public:
    void resize(int rows, int cols);

    bool load(const std::string& filename, uint32_t address, int rows, int cols, bool topToBottom);
    bool save();

    void clear();

    Tile get(int row, int col) const;
    void set(int row, int col, Tile tile);

    int rows() const { return mapRows; }
    int cols() const { return mapCols; }
    bool hasLoaded() const { return loaded; }
    std::string getFilename() const { return filename;};

private:
    std::vector<std::vector<Tile>> data;
    int mapRows = 0;
    int mapCols = 0;
    uint32_t address = 0x0000;
    bool topToBottom = true;
    std::string filename = "no file";
    bool loaded = false;

    void loadTopToBottom(std::ifstream& file);
    void loadLeftToRight(std::ifstream& file);
    void saveTopToBottom(std::fstream& file);
    void saveLeftToRight(std::fstream& file);
};
