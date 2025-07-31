#pragma once
#include <string>
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

void LoadConfig(const std::string& filename);
void ApplyConfig();

struct GameProfile {
    std::string name;
    std::string configPath;
};

struct EditorConfig {
    int tileSize = 64;
    int tilesetTotal = 320;

    int mapRows = 16;
    int mapCols = 16 * 84;

    int tilemapAddress = 0x190000;
    bool topToBottom = true;

    std::string tilesetFilename = "./Games/Template Game/template-game-tileset.png";
    std::string mapFilename     = "./Games/Template Game/template-game.sfc";
};

extern EditorConfig config;

// Add this to enable automatic parsing:
inline void from_json(const nlohmann::json& j, EditorConfig& c) {
    j.at("tile_size").get_to(c.tileSize);
    j.at("tileset_total").get_to(c.tilesetTotal);
    j.at("map_rows").get_to(c.mapRows);
    j.at("map_cols").get_to(c.mapCols);
    j.at("tilemap_address").get_to(c.tilemapAddress);
    j.at("top_to_bottom").get_to(c.topToBottom);
    j.at("tileset_filename").get_to(c.tilesetFilename);
    j.at("map_filename").get_to(c.mapFilename);
}

inline void to_json(json& j, const EditorConfig& c) {
    j = json{
        {"tile_size", c.tileSize},
        {"tileset_total", c.tilesetTotal},
        {"map_rows", c.mapRows},
        {"map_cols", c.mapCols},
        {"tilemap_address", c.tilemapAddress},
        {"top_to_bottom", c.topToBottom},
        {"tileset_filename", c.tilesetFilename},
        {"map_filename", c.mapFilename}
    };
}

extern std::vector<GameProfile> gGameProfiles;
extern int gSelectedGameIndex;
std::string SelectGameConfig();


extern json gCurrentConfig;
extern std::string gCurrentConfigPath;

enum class RenderDirection {
    Down,
    Right
};
