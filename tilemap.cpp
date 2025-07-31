#include "tilemap.h"
#include <iostream>

void Tilemap::resize(int rows, int cols) {
    data.resize(rows);
    for (auto& row : data) {
        row.resize(cols);
    }
    mapRows = rows;
    mapCols = cols;

    std::cout << "Resizing tilemap to " << rows << " rows and " << cols << " cols\n";
}

Tile Tilemap::get(int row, int col) const {
    return data[row][col];
}

void Tilemap::set(int row, int col, Tile tile) {
    data[row][col] = tile;
}

 void Tilemap::load(const std::string& filename, uint32_t address, int rows, int cols, bool topToBottom) {
    this->filename = filename;
    this->address = address;
    resize(rows, cols);
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open " << filename << "!\n";
        return;
    }

    file.seekg(address, std::ios::beg);
    if (topToBottom)
        loadTopToBottom(file);
    else
        loadLeftToRight(file);
}

void Tilemap::save() {
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "[Save] Could not open " << filename << " for writing!\n";
        return;
    }

    file.seekp(address, std::ios::beg);
    if (topToBottom)
        saveTopToBottom(file);
    else
        saveLeftToRight(file);
}

void Tilemap::loadTopToBottom(std::ifstream& file) {
    for (int col = 0; col < mapCols; ++col) {
        for (int row = 0; row < mapRows; ++row) {
            uint8_t lo = 0, hi = 0;
            file.read(reinterpret_cast<char*>(&lo), 1);
            file.read(reinterpret_cast<char*>(&hi), 1);
            data[row][col].raw = lo | (hi << 8);
        }
    }
    loaded = true;
}

void Tilemap::loadLeftToRight(std::ifstream& file) {
    for (int row = 0; row < mapRows; ++row) {
        for (int col = 0; col < mapCols; ++col) {
            uint8_t lo = 0, hi = 0;
            file.read(reinterpret_cast<char*>(&lo), 1);
            file.read(reinterpret_cast<char*>(&hi), 1);
            data[row][col].raw = lo | (hi << 8);
        }
    }
    loaded = true;
}

void Tilemap::saveTopToBottom(std::fstream& file) {
    for (int col = 0; col < mapCols; ++col) {
        for (int row = 0; row < mapRows; ++row) {
            Tile v = data[row][col];
            uint8_t lo = v.raw & 0xFF, hi = (v.raw >> 8) & 0xFF;
            file.write(reinterpret_cast<char*>(&lo), 1);
            file.write(reinterpret_cast<char*>(&hi), 1);
        }
    }
}

void Tilemap::saveLeftToRight(std::fstream& file) {
    for (int row = 0; row < mapRows; ++row) {
        for (int col = 0; col < mapCols; ++col) {
            Tile v = data[row][col];
            uint8_t lo = v.raw & 0xFF, hi = (v.raw >> 8) & 0xFF;
            file.write(reinterpret_cast<char*>(&lo), 1);
            file.write(reinterpret_cast<char*>(&hi), 1);
        }
    }
}

void Tilemap::clear() {
    for (int col = 0; col < mapCols; ++col) {
        for (int row = 0; row < mapRows; ++row) {
            data[row][col] =Tile{0,false,false};
        }
    }
}
