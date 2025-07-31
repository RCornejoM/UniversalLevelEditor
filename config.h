#pragma once
#include <string>
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

#define JSON_GET_OR(j, key, var, def) \
    var = (j.contains(key) && !j[key].is_null()) ? j[key].get<decltype(var)>() : def;


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
    JSON_GET_OR(j, "tile_size",        c.tileSize,       8);
    JSON_GET_OR(j, "tileset_total",    c.tilesetTotal,   0);
    JSON_GET_OR(j, "map_rows",         c.mapRows,        0);
    JSON_GET_OR(j, "map_cols",         c.mapCols,        0);
    JSON_GET_OR(j, "tilemap_address",  c.tilemapAddress, 0u);
    JSON_GET_OR(j, "top_to_bottom",    c.topToBottom,    false);
    JSON_GET_OR(j, "tileset_filename", c.tilesetFilename, std::string{});
    JSON_GET_OR(j, "map_filename",     c.mapFilename,    std::string{});
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
