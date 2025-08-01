#pragma once
#include <vector>
#include <cstdint>
#include "engine/tilemap.h"

class Stamp {
public:
    void copyFromTilemap(int row1, int col1, int row2, int col2);
    void pasteToTilemap(int row, int col);
    void setSingleTile(uint16_t tileData);
    std::pair<int,int> getCenter(int centerRow, int centerCol) const;
    void flipHorizontally();
    void flipVertically();

    int rows() const { return stampRows; }
    int cols() const { return stampCols; }
    Tile get(int row, int col) const { return data[row][col]; }

private:
    std::vector<std::vector<Tile>> data;
    int stampRows = 0, stampCols = 0;
};
