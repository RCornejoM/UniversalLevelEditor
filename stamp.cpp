#include "stamp.h"
#include "globals.h"
#include <iostream>
#include <algorithm>

// --- For tile picking logic ---
bool gPickingFromPalette = false;

std::vector<std::vector<Tile>> copiedTiles;

void Stamp::copyFromTilemap(int row1, int col1, int row2, int col2) {
    int r0 = std::min(row1, row2), r1 = std::max(row1, row2);
    int c0 = std::min(col1, col2), c1 = std::max(col1, col2);

    data.clear();
    for (int r = r0; r <= r1; ++r) {
        std::vector<Tile> row;
        for (int c = c0; c <= c1; ++c) {
            row.push_back(tilemap->get(r,c));
        }
        data.push_back(row);
    }
    stampRows = r1 - r0 + 1;
    stampCols = c1 - c0 + 1;
    placingStamp = false;
    std::cout << "Copied " << stampRows << "x" << stampCols << " region into stamp.\n";
}

void Stamp::pasteToTilemap(int centerRow, int centerCol) {
    auto [startRow, startCol] = getCenter(centerRow, centerCol);
    for (int r = 0; r < stampRows; ++r) {
        for (int c = 0; c < stampCols; ++c) {
            int dstRow = startRow + r;
            int dstCol = startCol + c;
            if (dstRow >= 0 && dstRow < config.mapRows && dstCol >= 0 && dstCol < config.mapCols) {
                tilemap->set(dstRow, dstCol, data[r][c]);
            }
        }
    }
}

void Stamp::setSingleTile(uint16_t tileData) {
    data.clear();
    data.push_back({ tileData });

    stampRows = 1;
    stampCols = 1;
    placingStamp = false;

    std::cout << "Set 1x1 stamp with tile 0x" << std::hex << (tileData & 0xFFFF)
              << " (" << std::dec << (tileData & 0xFFFF) << ")\n";
}

std::pair<int,int> Stamp::getCenter(int centerRow, int centerCol) const {
    int topLeftRow = centerRow - (stampRows / 2);
    int topLeftCol = centerCol - (stampCols / 2);
    return {topLeftRow, topLeftCol};
}

void Stamp::flipHorizontally() {
    for (auto& row : data) {
        std::reverse(row.begin(), row.end());
        for (auto& tile : row) {
            tile.flipHorizontally();
        }
    }
}

void Stamp::flipVertically() {
    std::reverse(data.begin(), data.end());
    for (auto& row : data) {
        for (auto& tile : row) {
            tile.flipVertically();
        }
    }
}
